#ifndef SERVER_H
#define SERVER_H
//wasif legendary 128.100.13.132 5000
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include<sys/socket.h>
#include <sys/types.h>
#include<arpa/inet.h>
#include <unistd.h> 


char buff[2000]; 
struct Message {
    unsigned int type;
    unsigned int size;
    unsigned char source[20];
    unsigned char data[1000];
};
int requestLoop(int connfd);
int processPacket(char* packet);
int responseHandler(const struct Message);
void debugger(int code);

int loginClient(char* credentials);
int logoutClient();
int joinSesson();
int leaveSession();
int newSession(int sessionID);
int listStatus();



#endif

