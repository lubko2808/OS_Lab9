#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <cstring>
#include <vector>

const int MaxClients = 100;
const int MaxMessageLength = 260;
int MaxBadWord = 0;
std::vector<std::string> badWord;
int** g_d;
CRITICAL_SECTION criticalSection;
HANDLE semaphore;

bool** correct;
bool** messagesChecked;

//------------------------------ "User data for current moment"
struct UserData {
    int id;
    char nickname[60];
    char password[60];
    char messages[MaxMessageLength];
    bool signal;
    bool inUse;
};

struct UsersLog {

    UserData user[MaxClients];
};
//------------------------------

//------------------------------ "History of posted messages"
struct MessagesLog {
    char message[100000][MaxMessageLength];
    bool correct[100000];
    bool canBeRead[100000];
};

//------------------------------

MessagesLog* messages;
UsersLog* users;
void prefixFunction(std::string key, int* d) {
    int currJointSize = 0; // довжина поточного співпадіння префікса та суфікса
    

    int i = 1; // d[0] = 0
    while (i < key.size()) {
        if (key[i] == key[currJointSize]) {
            currJointSize++;
            d[i] = currJointSize;
            i++;
        }
        else {
            if (currJointSize != 0) {
                currJointSize = d[currJointSize - 1];
            }
            else {
                d[i] = 0;
                i++;
            }
        }
    }
}



int searchWord(std::string text, std::string word, int* d) {
    int textIndex = 0; // Індекс для тексту
    int wordIndex = 0; // Індекс для слова
    int numOfBadWords = 0;
    //std::cout << "In search";
    while (textIndex < text.size()) {
        if (word[wordIndex] == text[textIndex]) {
            textIndex++;
            wordIndex++;
        }
        if (wordIndex == word.size()) {
            numOfBadWords++;
            wordIndex = d[wordIndex - 1]; // Використовуємо префікс-функцію для зміщення слова
        }
        else if (textIndex < text.size() && word[wordIndex] != text[textIndex]) {
            if (wordIndex != 0) { // Використовуємо префікс-функцію для зміщення слова
                textIndex -= d[wordIndex - 1];
                wordIndex = 0;
            }
            else {
                textIndex++;
            }
        }
    }
    return numOfBadWords;
}

void readBadWord(std::string filePath)
{
    std::ifstream inputFile(filePath);

    // Перевірка, чи файл вдалося відкрити
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file\n";
        return; // Повертаємо код помилки
    }

    // Зчитування слів з файлу та додавання їх до масиву
    std::string word;
    while (inputFile >> word) {
        badWord.push_back(word);
        MaxBadWord++;
    }

    

    // Закриття файлу
    inputFile.close();

    // Виведення слів з масиву (для перевірки)
    std::cout << "Bad Words:\n";
    for (const auto& badWord : badWord) {
        std::cout << badWord << "\n";
    }




    return;
}

DWORD findBadWord(LPVOID param) {
    int* data = reinterpret_cast<int*>(param);
    int numOfModer = data[0], numOfMes = data[1];

    std::string m(messages->message[numOfMes]);
    std::string bm(badWord[numOfModer]);

    // Знаходимо позицію першої двокрапки
    size_t colonPos = m.find(':');
    // Перевіряємо, чи знайдено двокрапку
    if (colonPos != std::string::npos) {
        // Якщо знайдено, починаємо пошук від позиції двокрапки
        m = m.substr(colonPos);
    }

    correct[numOfMes][numOfModer] = searchWord(m, bm, g_d[numOfModer]);

   
    std::cout << "Message was checked by " << numOfModer << ". Num of bad words: " << correct[numOfMes][numOfModer] << std::endl;


    messagesChecked[numOfMes][numOfModer] = true;

    delete[] data;

    return 0;
}



DWORD checkMessage(LPVOID param)
{
    int id = reinterpret_cast<int>(param);
    //int data1[2], data2[2], data3[2];
    int* data;
    int messNum = 0;
    int sum = 0;

    EnterCriticalSection(&criticalSection);
    for (int i = 0; i < 100000; i++)
    {
        if (messages->message[i][0] == '\0' &&
            messages->message[i][1] == '\0')
        {
            //std::cout << "before copy";
            strcpy_s(messages->message[i], MaxMessageLength, users->user[id].messages);
           
            //std::cout << "after copy";
            std::cout << "Message was added to log\n";

            messNum = i;
            for (int j = 0; j < MaxBadWord; j++)
            {
                //std::cout << "befor create thread";
                data = new int[2];
                data[0] = j;
                data[1] = i;
                CreateThread(NULL, 0, findBadWord, reinterpret_cast<LPVOID>(data), 0, NULL);
                //std::cout << "after creat thread";
            }

            break;
        }
    }
    LeaveCriticalSection(&criticalSection);


    
    for(int i = 0; i < MaxBadWord; i++)
    {
        if (messagesChecked[messNum][i] == true)
        {
            sum++;
        }

        if (sum == MaxBadWord)
        {
            sum = 0;
            break;
        }
        else
        {
            i = 0;
        }
    }

  
    for (int i = 0; i < MaxBadWord; i++)
    {
        sum += (int)(correct[messNum][i]);
    }

 

    if (sum >= 2)
    {
        messages->correct[messNum] = false;
        std::cout << "Incorrect message\n";
    }
    else
    {
        messages->correct[messNum] = true;
        std::cout << "Correct message\n";
    }

    messages->canBeRead[messNum] = true;
   
    return 0;
}

void saveUserInCatalog(UserData &user) {
    HANDLE hFile = CreateFileA(
        "UserCatalog.txt",                          // Ім'я файлу
        FILE_APPEND_DATA,                           // Режим: додавання до файлу
        FILE_SHARE_WRITE | FILE_SHARE_READ,         // Дозвіл на спільний доступ
        NULL,                                       // За замовчуванням
        OPEN_ALWAYS,                                // Відкриваємо файл, якщо існує
        FILE_ATTRIBUTE_NORMAL,                      // Звичайний файл
        NULL                                        // Шаблон для файлу
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Unable to open UserCatalog.txt for writing!" << std::endl;
        return;
    }

    DWORD bytesWritten;
    std::string userData;
    userData = std::string(user.nickname) + " : " + std::string(user.password) + "\n";

    WriteFile(
        hFile,                              // Дескриптор файлу
        userData.c_str(),                   // Дані для запису
        userData.length(),                  // Розмір даних
        &bytesWritten,                      // Кількість записаних байтів
        NULL                                // За замовчуванням
    );

    CloseHandle(hFile);
}
void loadUserCatalog(UsersLog** users) {
    HANDLE hFile = CreateFileA(
        "UserCatalog.txt",                          // Ім'я файлу
        GENERIC_READ,                               // Режим: читання
        FILE_SHARE_READ | FILE_SHARE_WRITE,         // Дозвіл на спільний доступ
        NULL,                                       // За замовчуванням
        OPEN_EXISTING,                              // Відкриваємо тільки існуючий файл
        FILE_ATTRIBUTE_NORMAL,                      // Звичайний файл
        NULL                                        // Шаблон для файлу
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Unable to open UserCatalog.txt for reading!" << std::endl;
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Error getting file size!" << std::endl;
        CloseHandle(hFile);
        return;
    }

    char* buffer = new char[fileSize + 1];
    DWORD bytesRead;

    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
        std::cerr << "Error reading from file!" << std::endl;
        CloseHandle(hFile);
        delete[] buffer;
        return;
    }

    buffer[bytesRead] = '\0'; // Додаємо завершуючий нуль для забезпечення коректного рядка

    CloseHandle(hFile);

    std::string data(buffer);
    delete[] buffer;

    size_t startPos = 0;
    size_t endPos;
    int currEmptyUserIndex = 0;
    while ((endPos = data.find("\n", startPos)) != std::string::npos) {
        std::string line = data.substr(startPos, endPos - startPos);
        size_t delimiterPos = line.find(" : ");

        if (delimiterPos != std::string::npos) {
            std::string nickname = line.substr(0, delimiterPos);
            std::string password = line.substr(delimiterPos + 3);

            // Знаходження першого вільного місця для зберігання користувача
            int freeIndex = -1;
            while (currEmptyUserIndex < MaxClients) {
                if ((*users)->user[currEmptyUserIndex].nickname[0] == '\0' && (*users)->user[currEmptyUserIndex].password[0] == '\0') {
                    freeIndex = currEmptyUserIndex;
                    break;
                }
                currEmptyUserIndex++;
            }

            if (freeIndex != -1) {
                strcpy_s((*users)->user[freeIndex].nickname, MaxMessageLength, nickname.c_str());
                strcpy_s((*users)->user[freeIndex].password, MaxMessageLength, password.c_str());
            }
        }

        startPos = endPos + 1;
    }
}














int main() {
    //____________________MESSAGES LOG FILE_____________________
    HANDLE fileMessagesLog = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(MessagesLog),
        L"MessagesLog");

    if (fileMessagesLog == NULL) {
        std::cerr << "Mapping file was not created (log of messages): " << GetLastError() << std::endl;
        return 1;
    }

    messages = (MessagesLog*)MapViewOfFile(
        fileMessagesLog,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(MessagesLog));

    if (messages == NULL) {
        std::cerr << "Error with getting point on file mapping: " << GetLastError() << std::endl;
        CloseHandle(messages);
        return 1;
    }

    //____________________USERS DATA FILE______________________

    HANDLE fileUsersLog = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(UsersLog),
        L"UsersLog");

    if (fileUsersLog == NULL) {
        std::cerr << "Mapping file was not created (log of users): " << GetLastError() << std::endl;
        return 1;
    }
    users = (UsersLog*)MapViewOfFile(
        fileUsersLog,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(UsersLog));

    if (users == NULL) {
        std::cerr << "Error with getting point on file mapping: " << GetLastError() << std::endl;
        CloseHandle(users);
        return 1;
    }
    //________________________________________________________

    
    for (int i = 0; i < 100000; i++)
    {
        messages->correct[i] = 0;
        messages->canBeRead[i] = 0;
        messages->message[i][0] = '\0';
        messages->message[i][1] = '\0';
    }

    for (int i = 0; i < MaxClients; ++i) {
        users->user[i].id = i;
        users->user[i].inUse = false;
        users->user[i].signal = false;
        users->user[i].nickname[0] = '\0';
        users->user[i].password[0] = '\0';

    }

    loadUserCatalog(&users); // завантаження записів про користувачів із файлу
    readBadWord(std::string("BadWordList.txt"));
 
    g_d = new int* [MaxBadWord]; // змінна для префікс-функції
    for (int i = 0; i < MaxBadWord; i++) {
        g_d[i] = new int[20];
        for (int j = 0; j < 20; j++)
        {
            g_d[i][j] = 0;
        }
    }

    correct = new bool* [100000];
    messagesChecked = new bool* [100000];

    for (int i = 0; i < 100000; i++)
    {
        correct[i] = new bool[MaxBadWord];
        messagesChecked[i] = new bool[MaxBadWord];
    }

    for (int i = 0; i < 100000; i++)
    {
        for (int j = 0; j < MaxBadWord; j++)
        {
            correct[i][j] = false;
            messagesChecked[i][j] = false;
        }
    }

    for (int i = 0; i < MaxBadWord; i++) {
        //std::string key = badWord[i];
        prefixFunction(badWord[i], g_d[i]);
    }
    InitializeCriticalSection(&criticalSection);
    

    //________________________________________________________

    std::string bufstr;
    std::cout << "Input topic (letters): ";
    std::getline(std::cin, bufstr);
    for (int i = 0; i < MaxMessageLength; i++)
    {
        messages->message[0][i] = bufstr.c_str()[i];
    }
    messages->message[0][MaxMessageLength-1] = '\0';

    std::cout << "SERVER IS READY" << std::endl;




    

    while (true) {
        for (int i = 0; i < MaxClients; ++i) {
            

            if (users->user[i].signal == true) {
                std::cout << "New message from client " << users->user[i].messages << std::endl;
                CreateThread(NULL, 0, checkMessage, reinterpret_cast<LPVOID>(i), 0, NULL);


                users->user[i].signal = false;
            }
            if (users->user[i].inUse == true)
            {
                // write in file.txt
                saveUserInCatalog(users->user[i]);
                users->user[i].inUse = false;
            }
        }

    }








    

    UnmapViewOfFile(users);
    CloseHandle(fileUsersLog);

    UnmapViewOfFile(messages);
    CloseHandle(fileMessagesLog);

    return 0;
}
