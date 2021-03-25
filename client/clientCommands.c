#include "client.h"

int login (char* clientID, char* password, char* serverIP, char* serverPort){
    // error check to make sure each value was entered
    // will need to error check if the values work and error check that aswell 
    if (clientID == NULL || password == NULL || serverIP == NULL || serverPort == NULL){
        return -1;
    }

    //Establish connection with server based on credentials from user
    int ret = establishConnection(clientID, password, serverIP, serverPort);
    return 0;
}

int logout(){
    int ret = msgSender(6, 0, username, NULL, clientFD);
    return 0;
}

int joinsession(char* sessionID){
    //printf("%s, %d\n",username, clientFD);
    int ret = msgSender(4, strlen(sessionID), username, sessionID, clientFD);
    return 0;
    //return 0;
}

int leavesession(char* sessionID){
    //printf("%s, %d\n",username, clientFD);
    int ret = msgSender(3, strlen(sessionID), username, sessionID, clientFD);
    return 0;
}

int createsession(char* sessionID){
    //printf("%s, %d\n",username, clientFD);
    int ret = msgSender(2, strlen(sessionID), username, sessionID, clientFD);
    return 0;
}

int list(){
    int ret = msgSender(8, 0, username, NULL, clientFD);
    return 0;
}

void prompter (){
    printf("\n>> ");
}

void debugger(int code){
    printf("you made it here: %d\n", code);
}

void help(){
    printf("Hey! Here's a list of commands and the information they'll require:\n\n");
    printf("command: /login\n");
    printf("requrements: client ID, password, server-IP, server-port\n\n");
    printf("command: /logout\n");
    printf("requrements: none\n\n");
    printf("command: /joinsession\n");
    printf("requrements: sessionID\n\n");
    printf("command: /leavesession\n");
    printf("requrements: none\n\n");
    printf("command: /createsession\n");
    printf("requrements: sessionID\n\n");
    printf("command: /list\n");
    printf("requrements: none\n\n");
    printf("command: /quit\n");
    printf("requrements: none\n\n");
}

void texter(char* text) {
    //debugger(2);
    //printf("%s\n", text);
    int ret = msgSender(7, strlen(text), username, text, clientFD);
}