
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

// each function call has to be error checked, particularly the ones that take input 
// return -1 if the values didnt work in a networks part of it 
// return 0 if arguments are missing 
// return 1 if everything works fine. send return into a error handling function that outputs commands based on the issue 
// error handling function can ask user if they want to try again or quit or something like that 

// client interace  
int main (int argc, char *argv[]){
    printf("\nWelcome to TextConf by Wasif and Nissar!\n\n");
    printf("HelperBot: You can get started by typing 'help' or if you know your way around, get started!\n");
    loginFlag = 0; 
    while (1){
        char command[50]; 
        prompter();
        scanf("%s",command);
        printf("\n");
        if (strcmp(command, "help") == 0){
            help();
        } else if (strcmp(command, "/login") == 0){
            printf("HelperBot: Logging you in! Please enter your client ID, password, server-IP and server-port separated by spaces:\n");
            prompter();
            scanf(" %s %s %s %s", clientID, password, serverIP, serverPort);
            login(clientID, password, serverIP, serverPort);
        } else if (strcmp(command, "/logout") == 0){ //send logout request and uninitialize the global variables
            printf("HelperBot: Logging you out of the server!\n");
            logout();
        }else if (strcmp(command, "/joinsession") == 0){
            printf("Type in your session ID and join your session!\n");
            prompter();
            scanf(" %s", sessionID);
            joinsession(sessionID);
        }else if (strcmp(command, "/leavesession") == 0){
            printf("HelperBot: Leaving your session. \n");
            leavesession(sessionID);
        }else if (strcmp(command, "/createsession") == 0){
            printf("HelperBot: Type in a valid session ID to start your session!\n");
            prompter();
            scanf(" %s", sessionID);
            createsession(sessionID);
        }else if (strcmp(command, "/list") == 0){
            printf("HelperBot: Listing connected clients and available sessions:\n");
            list();
        }else if (strcmp(command, "/quit") == 0){ // calls logout and quits program 
            printf("HelperBot: Quitting TextConf. See you soon!\n");
            exit(0);
        } else if(strcmp(command, "<text>") == 0){
            printf("HelperBot: Type a message to send to your session!\n");
            prompter();
            fgets(text,200,stdin);
            texter(text);
        }
        else {
            printf("HelperBot: Unrecognized command! Please try again or type 'help' for a list of commands!\n");
        }
    }



    return -1;
}