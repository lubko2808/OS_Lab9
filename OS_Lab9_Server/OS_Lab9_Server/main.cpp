#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <string>
#include <cstring>
#include <vector>

const int MaxClients = 100;
const int MaxMessageLength = 200;
const int MaxBadWord = 3;
char*** badWord;
CRITICAL_SECTION criticalSection;

bool correct[100000][MaxBadWord];
bool messagesChecked[100000][MaxBadWord];

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

DWORD findBadWord(LPVOID param)
{
    int* data = reinterpret_cast<int*>(param);
    int numOfModer = data[0], numOfMes = data[1];

    std::string m(messages->message[numOfMes]);
    std::string bm(badWord[numOfMes][numOfModer]);

    size_t found = m.find(bm);

    if (found != std::string::npos) {
        correct[numOfMes][numOfModer] = true;
    }

    std::cout << "Message was checked" << numOfModer << '\n';

    /*if (std::strstr(messages->message[numOfMes], badWord[numOfMes][numOfModer]) != nullptr) {
        correct[numOfMes][numOfModer] = true;
    }*/

    messagesChecked[numOfMes][numOfModer] = true;

    return 0;
}



DWORD checkMessage(LPVOID param)
{
    int id = reinterpret_cast<int>(param);
    int data1[2], data2[2], data3[2];
    int sum = 0;

    EnterCriticalSection(&criticalSection);
    for (int i = 0; i < 100000; i++)
    {
        if (messages->message[i][0] == '\0' &&
            messages->message[i][1] == '\0')
        {
            strcpy_s(messages->message[i], 200, users->user[id].messages);
            data1[1] = i;
            data2[1] = i;
            data3[1] = i;
            std::cout << "Message was add to log\n";
            break;
        }
    }
    LeaveCriticalSection(&criticalSection);

    data1[0] = 0;
    data2[0] = 1;
    data3[0] = 2;


    CreateThread(NULL, 0, findBadWord, reinterpret_cast<LPVOID>(data1), 0, NULL);
    CreateThread(NULL, 0, findBadWord, reinterpret_cast<LPVOID>(data2), 0, NULL);
    CreateThread(NULL, 0, findBadWord, reinterpret_cast<LPVOID>(data3), 0, NULL);

    while (messagesChecked[data1[1]][0] != true && messagesChecked[data2[1]][1] != true && messagesChecked[data3[1]][2] != true)
    {
    }

    sum = (int)(correct[data1[1]][0]) + (int)(correct[data2[1]][2]) + (int)(correct[data3[1]][2]);

    if (sum > 2)
    {
        messages->correct[data1[1]] = false;
        std::cout << "uncorrect\n";
    }
    else
    {
        messages->correct[data1[1]] = true;
        std::cout << "correct\n";
    }

    messages->canBeRead[data1[1]] = true;

    return 0;
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
        for (int j = 0; j < MaxBadWord; j++)
        {
            correct[i][j] = 0;


        }
        messages->correct[i] = 0;
        messages->canBeRead[i] = 0;
        messages->message[i][0] = '\0';
        messages->message[i][1] = '\0';
    }

    for (int i = 0; i < MaxClients; ++i) {
        users->user[i].id = i;
        users->user[i].signal = false;
        users->user[i].nickname[0] = '\0';
        users->user[i].password[0] = '\0';

    }

    badWord = new char** [100000];

    for (int i = 0; i < 100000; i++)
    {
        badWord[i] = new char* [3];

        badWord[i][0] = new char[10];
        strcpy_s(badWord[i][0], 5, "here");

        badWord[i][1] = new char[10];
        strcpy_s(badWord[i][1], 5, "up");

        badWord[i][2] = new char[10];
        strcpy_s(badWord[i][2], 5, "gg");




    }

    InitializeCriticalSection(&criticalSection);

    /* for (int i = 0; i < 100000; i++)
     {
         for (int j = 0; j < MaxBadWord; j++)
         {
             events[i][j] = CreateEvent(NULL, TRUE, FALSE, (L"SharedMemoryEvent" + std::to_wstring(i) + std::to_wstring(j)).c_str());
         }
     }*/


     //________________________________________________________

    std::string bufstr;
    std::cout << "Input topic (200 letters): ";
    std::getline(std::cin, bufstr);
    for (int i = 0; i < MaxMessageLength; i++)
    {
        messages->message[0][i] = bufstr.c_str()[i];
    }
    messages->message[0][199] = '\0';

    std::cout << "SERVER IS READY" << std::endl;






    while (true) {
        for (int i = 0; i < MaxClients; ++i) {


            if (users->user[i].signal == true) {
                std::cout << "New message from client" << i << ": " << users->user[i].messages << std::endl;
                CreateThread(NULL, 0, checkMessage, reinterpret_cast<LPVOID>(i), 0, NULL);


                users->user[i].signal = false;
            }
        }

    }










    UnmapViewOfFile(users);
    CloseHandle(fileUsersLog);

    UnmapViewOfFile(messages);
    CloseHandle(fileMessagesLog);

    return 0;
}
