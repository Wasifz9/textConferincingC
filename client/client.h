#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#define _GNU_SOURCE
#define MAX_SESSION_MEMS 8
#define MAX_SESSIONS_JOINED 3
#define MAX_SESSIONS 8 
#define MAX_CLIENTS 32 
#define MAX_CLIENT_ID 20
#define MAX_CLIENT_PASS 10
#define MAX_DATA_SIZE 1000


struct Message{
    unsigned int type;
    unsigned int size;
    unsigned char source[MAX_CLIENT_ID];
    unsigned char data[MAX_DATA_SIZE];
};
int login(char *clientID, char *password, char *serverIP, char *serverPort);
int logout();
int joinsession(char* sessionID);
int leavesession();
int createsession(char* sessionID);
int list();
void prompter();
void debugger(int code);
void help();
char* msgReader(struct Message msg);
int msgSender (int type, unsigned int size, char* source, char* data, int connfd);
int establishConnection(char* clientID, char* password, char* serverIP, char* serverPort);
char buff[1000]; 



#endif