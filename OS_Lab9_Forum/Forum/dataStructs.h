#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <Windows.h>
#include <cstring>

const int MaxClients = 100;
const int MaxMessageLength = 200;


//------------------------------ "User data for current moment"
struct UserData {
    int id;
    char nickname[100];
    char password[100];
    char messages[MaxMessageLength];
    bool signal;
};

struct UsersLog {
    UserData user[MaxClients];
};
//----------------------------------------------------------------------

//------------------------------ "History of posted messages"
struct MessagesLog {
    char message[100000][MaxMessageLength];
    bool correct[100000];
    bool canBeRead[100000];
};

//-----------------------------------------------------------------------




#endif // DATASTRUCTS_H
