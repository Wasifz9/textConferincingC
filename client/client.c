
#include "client.h"
// required data for functions 
char clientID[100];
char password[100]; 
char serverIP[100];
char serverPort[100];
char sessionID[100];
char text[200];
// 128.100.13.132 
///////////
// NOTES //
///////////

// check input length and make sure char buffers dont overflow!

// client interace  
int main (int argc, char *argv[]){
    printf("\nWelcome to TextConf by Wasif and Nissar!\n\n");
    printf("HelperBot: You can get started by typing 'help' or if you know your way around, get started!\n");
    loginFlag = 0;
    sessFlag = 0; 
    while (1){
        char command[50]; 
        //prompter();
        scanf("%s",command);
        printf("\n");
        if (strcmp(command, "/help") == 0){
            help();
        } else if (strcmp(command, "/login") == 0){ 
            scanf(" %s %s %s %s", clientID, password, serverIP, serverPort);
            if (loginFlag == 0){
                //printf("HelperBot: Logging you in! Please enter your client ID, password, server-IP and server-port separated by spaces:\n");
                //prompter();
                
                login(clientID, password, serverIP, serverPort);
            } else {
                printf("HelperBot: You're already logged in! Logout to switch accounts!\n");
            }
        } else if (strcmp(command, "/logout") == 0){ //send logout request and uninitialize the global variables
            if (loginFlag == 1){
                //printf("HelperBot: Logging you out of the server!\n");
                logout();
            } else {
                printf("HelperBot: Login before logging out\n");
            }
        }else if (strcmp(command, "/joinsession") == 0){
            scanf(" %s", sessionID);
            if (sessFlag == 0){
                //printf("Type in your session ID and join your session!\n");
                //prompter();
                
                joinsession(sessionID);
            }else {
                printf("HelperBot: Leave your session first before you join a new session!\n");
            }
        }else if (strcmp(command, "/leavesession") == 0){
            if (sessFlag == 1){
                //printf("HelperBot: Leaving your session. \n");
                leavesession(sessionID);
            } else {
                printf("You are not in a session yet! Join one to start chatting!\n");
            }
        }else if (strcmp(command, "/createsession") == 0){
            scanf(" %s", sessionID);
            if (sessFlag == 0){
                //printf("HelperBot: Type in a valid session ID to start your session!\n");
                //prompter();
                createsession(sessionID);
            }else {
                printf("HelperBot: Leave your session first before you start a new session!\n");
            }
        }else if (strcmp(command, "/list") == 0){
            if (loginFlag == 1){
                //printf("HelperBot: Listing connected clients and available sessions:\n");
                list();
            } else {
                printf("HelperBot: Login to a server first!");
            }
        }else if (strcmp(command, "/quit") == 0){ // calls logout and quits program
            if (loginFlag == 1){
                logout(); 
            }
            printf("HelperBot: Quitting TextConf. See you soon!\n");
            exit(0);
        } // leave this if else for second part where messages can be sent to several sessions 
        /*else if(strcmp(command, "<text>") == 0){
            //printf("HelperBot: Type a message to send to your session!\n");
            //prompter();
            //fgets(text,200,stdin);
            char* text = "tester message I hope it works"; 
            texter(text);
        }*/ 
        else { //send message if first string isnt a command 
            //printf("HelperBot: Unrecognized command! Please try again or type 'help' for a list of commands!\n");
            if (loginFlag == 1 && sessFlag == 1){
                memset(text,NULL,200);
                strcpy(text, command);
                int comLength = strlen(command);
                fgets(text + comLength, 200 - comLength, stdin);
                texter(text);
            } else {
                printf("Join a session before sending a message. Type '/help' to get a list of available commands!\n");
            }
        }

        
    }
    return -1;
}

void * messageListener(){
    while(1){
        struct Message msg; 
        memset(&buff, 0, sizeof(buff)); 
        read(clientFD, buff, sizeof(buff));
        processPacket(buff, &msg);
        if (msg.type == 1){
            loginFlag = 1; 
            //printf("%s: Hey %s! %s\n", msg.source, username, msg.data);
        }
        if (msg.type == 2) { // 2
            printf("Log in failed.\n");
            printf("%s\n", msg.data);
            return;
        } else if (msg.type == 3) { // 3 
            printf("HelperBot: New session successfully created!\n");
            sessFlag = 1;
        } else if (msg.type == 4){ // 4
            printf("HelperBot: Session joined successfully!\n"); 
            sessFlag = 1;
        } else if (msg.type == 5){ // 5 
            printf("HelperBot: Couldn't join session!\n");
            printf("%s\n", msg.data);
        } else if (msg.type == 6){ // 6
            printf("HelperBot: Left session!\n");
            sessFlag = 0;
        } else if (msg.type == 7){ // 7
            printf("HelperBot: Succesfully logged out of the server! \n");
            loginFlag = 0;
            sessFlag = 0;
            return;
        } else if (msg.type == 8){
            //printf("HelperBot: Printing server list!\n");
            printf("ServerStatus: %s\n", msg.data);
        } 
        else if (strcmp(buff, "TXT_ACK") == 0){ // not doing anything rn
            printf("Text sent\n!");
        } 
        else {
            struct Message msg;
            processPacket(buff, &msg);
            printf("%s: %s\n", msg.source, msg.data);
        }

        //printf("\n\n");
        //prompter();
    }
}