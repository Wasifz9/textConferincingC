#ifndef SERVER_H
#define SERVER_H
//wasif legendary 128.100.13.132 5000
//nissar pathetic 128.100.13.132 5000
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include<sys/socket.h>
#include <sys/types.h>
#include<arpa/inet.h>
#include <unistd.h> 
#define MAX_SESSION_MEMS 8
#define MAX_SESSIONS_JOINED 3
#define MAX_SESSIONS 8 
#define MAX_CLIENTS 32 
#define MAX_CLIENT_ID 10
#define MAX_CLIENT_PASS 10
#define MAX_DATA_SIZE 1000


struct Server {
    struct Session* sessions[MAX_SESSIONS];
    struct Client* clients[MAX_CLIENTS];
};

struct Session {
    unsigned int sID;
    struct Client* clients[MAX_SESSION_MEMS];
};

struct Client {
    unsigned cId; // index 
    char username[MAX_CLIENT_ID]; // same as ClientID from client perspective 
    char password[MAX_CLIENT_PASS];
    unsigned int sessionJoined; // later make array of sessions joined 
    char IPaddr[20];
    int connfd; 
};

char buff[2000]; 
struct Message {
    unsigned int type;
    unsigned int size;
    unsigned char source[MAX_CLIENT_PASS]; //Client ID 
    unsigned char data[MAX_DATA_SIZE];
};


int requestLoop(int connfd);
int processPacket(char* packet);
int responseHandler(const struct Message);
void debugger(int code);

// server commands 
int loginClient(char* credentials);
int logoutClient();
int joinSession(const struct Message);
int leaveSession(const struct Message);
int newSession(int sessionID);
int listStatus();



#endif

