#include "server.h"

// allowed people 
const char *acceptedClientIDs[] = {"wasifz9","nissar","instructor"}; // will need to code data structures 
const char *acceptedClientPasswords[] = {"password", "pathetic", "allknowing"}; // will need to code data structures 


int loginClient(struct Server* sv, const struct Message msg, int connfd){
    // check client credentials against authorized credentials
    // check client credentials against real time connected users 
    unsigned int member = 1;
    char *username;   //member 1
    char *password;         //member 2

    int i1 = 0; //first index of a member
    int i2 = 0; //index of colon after the member

    for (; (i2 < 2000) && (member < 3); i2++)
    {
        if (msg.data[i2] == ',' || msg.data[i2] == '\0'){
            if (member == 1)    //total_frag
            {
                username = malloc(i2 - i1);
                memcpy(username, msg.data + i1, i2 - i1);
            }
            else if (member == 2)    //size
            {
                password = malloc(i2 - i1);
                memcpy(password, msg.data + i1, i2 - i1);
            }
            else{printf("error in credential parsing.\n"); exit(1);}

            i1 = i2 + 1;
            member++;
        }
    }

    printf("A client is trying to join with the credentals: \n username: %s\n password: %s\n", username, password);

    for (int i = 0; i < ACCEPTED_CLIENTS; i++){
        if (strcmp(acceptedClientIDs[i], username) == 0){
            if (strcmp(acceptedClientPasswords[i], password) == 0){ // need to send the prints as LO_NAKS
                printf("User has been authorized!\n");
                /// init user and add too server struct lists 
                // send appropriat LO_ACK
                client_init(sv, msg, connfd);
                return 1;
            }else{
                printf("Incorrect password for client!\n");
                // SEND LO_NAK
                return 1;
            }
        }
    }
    printf("User not found!\n");
    free(username);
    free(password);
    return 1;
    
}

int joinSession(const struct Message msg){

}

int leaveSession(const struct Message msg){

}

void client_init(struct Server* sv, const struct Message msg, int connfd){
    struct Client* cli = malloc(sizeof(struct Client));
    strcpy(cli->username,msg.source);
    cli->sessionJoined = -1; //-1 means hasnt joined one yet 
    cli->connfd = connfd;

    for (int i = 0; i<ACCEPTED_CLIENTS; i++){
        if (sv->clients[i] == NULL){
            sv->clients[i] = cli;
            cli->cId = i; 
        }
    }
}

