#ifndef SERVER_H
#define SERVER_H

//WASIF GAY

//wasifz9 password 128.100.13.132 5000 // only this one works everytime
//nissar pathetic 128.100.13.132 5000  
//instructor allknowing 128.100.13.132 5000
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
#define MAX_CLIENTS 3
#define MAX_CLIENT_ID 20
#define MAX_CLIENT_PASS 20
#define MAX_DATA_SIZE 1000
#define ACCEPTED_CLIENTS 3

struct Server {
    struct Session* sessions[MAX_SESSIONS];
    struct Client* clients[MAX_CLIENTS];
    int activeSessions;
    int activeClients; 
};

struct Session {
    unsigned int sID;
    struct Client* clients[MAX_SESSION_MEMS];
};

struct Client {
    char username[MAX_CLIENT_ID]; // same as ClientID from client perspective 
    unsigned int cId; 
    int sessionJoined; // later make array of sessions joined 
    int connfd; 
};

char buff[2000]; 
struct Message {
    unsigned int type;
    unsigned int size;
    unsigned char source[20]; //Client ID 
    unsigned char data[1000];
};

// server commands 
int loginClient(struct Server* sv, const struct Message, int connfd);
int logoutClient();
int joinSession(const struct Message);
int leaveSession(const struct Message);
int newSession(int sessionID);
int listStatus();

// helpers
int eventHandler(struct Server* sv, int connfd);
void processPacket(char* packet, struct Message*);
void debugger(int code);

//server 
struct Server* server_init();

//client
void client_init(struct Server* sv, const struct Message, int connfd);

//server
struct Server* server_init(); 


#endif

