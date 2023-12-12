#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <Windows.h>
#include <cstring>

const int MaxClients = 100;
const int MaxMessageLength = 200;
const int MaxMessageWithUserLength = 260;


//------------------------------ "User data for current moment"
struct UserData {
    int id;
    char nickname[60];
    char password[60];
    char messages[MaxMessageWithUserLength];
    bool signal;
    bool inUse;
};

struct UsersLog {
    UserData user[MaxClients];
};
//----------------------------------------------------------------------

//------------------------------ "History of posted messages"
struct MessagesLog {
    char message[100000][MaxMessageWithUserLength];
    bool correct[100000];
    bool canBeRead[100000];
};

//-----------------------------------------------------------------------




#endif // DATASTRUCTS_H
