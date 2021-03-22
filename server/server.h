#ifndef SERVER_H
#define SERVER_H

// start with logout function which is segfaulting somwhere

//WASIF GAY

//wasifz9 password 128.100.13.132 5000 // only this one works everytime
//nissar pathetic 128.100.13.132 5000 
//instructo0r allknowing 128.100.13.132 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include<sys/socket.h>
#include <sys/types.h>
#include<arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>
#define MAX_SESSION_MEMS 8
#define MAX_SESSIONS_JOINED 3
#define MAX_SESSIONS 8 
#define MAX_CLIENTS 3
#define MAX_CLIENT_ID 20
#define MAX_CLIENT_PASS 20
#define MAX_DATA_SIZE 1000
#define ACCEPTED_CLIENTS 3
#define MAX_SESSION_NAME 20

struct Server {
    struct Session* sessions[MAX_SESSIONS];
    struct Client* clients[MAX_CLIENTS];
    int activeSessions;
    int activeClients; 
};

struct Server* sv;

struct Session {
    unsigned int sID; // internal
    char sessionID[MAX_SESSION_NAME]; // from client
    struct Client* clients[MAX_SESSION_MEMS];
    int memberCount; 
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
void loginClient(const struct Message, int connfd);
void logoutClient(const struct Message);
void joinSession(const struct Message);
void leaveSession(const struct Message);
void createSession(const struct Message);
void listStatus();

// helpers
void* eventHandler(int* conn_fd);
void processPacket(char* packet, struct Message*);
void debugger(int code);
int clientLookup(char* username);
int sessionLookup(char* sessionID);
int sessClientLookup(struct Session* sess, char* username);
void acknowledger(int connfd, char* ackToSend);

//server 
struct Server* server_init();

//client
void client_init(const struct Message, int connfd);

//session
void session_init(const struct Message msg, struct Client* cli); 


#endif

