#include "server.h"

// allowed people 
const char *acceptedClientIDs[] = {"wasifz9","nissar","instructor"}; // will need to code data structures 
const char *acceptedClientPasswords[] = {"password", "pathetic", "allknowing"}; // will need to code data structures 


void loginClient(const struct Message msg, int connfd){
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
                
                client_init(msg, connfd);
                return 1;
            }else{
                printf("Incorrect password for client!\n");
                acknowledger(connfd, "LO_NACK");
                // SEND LO_NAK
                return 1;
            }
        }
    }
    printf("User not found!\n");
    free(username);
    free(password);
    
}

void createSession(const struct Message msg){
    int cIndex = clientLookup(msg.source);
    session_init(msg, sv->clients[cIndex]);
}

void joinSession(const struct Message msg){
    //struct Client* cli = clientLookup(sv, msg.source);
    return 0;
}

void leaveSession(const struct Message msg){
    int sIndex = sessionLookup(msg.data);
    int scIndex = sessClientLookup(sv->sessions[sIndex], msg.source);
    acknowledger(sv->sessions[sIndex]->clients[scIndex]->connfd, "LS_ACK");
    sv->sessions[sIndex]->clients[scIndex] = NULL;
    sv->sessions[sIndex]->memberCount--;
    if (sv->sessions[sIndex]->memberCount == 0){
        sv->sessions[sIndex] = NULL;
    }
}

void client_init(const struct Message msg, int connfd){ 
    // need to error check this (clients already logged in) cleints full
    struct Client* cli = malloc(sizeof(struct Client));
    strcpy(cli->username,msg.source);
    cli->sessionJoined = -1; //-1 means hasnt joined one yet 
    cli->connfd = connfd;

    for (int i = 0; i<ACCEPTED_CLIENTS; i++){ // currently just filling next available spot 
        if (sv->clients[i] == NULL){
            sv->clients[i] = cli;
            cli->cId = i;
            acknowledger(cli->connfd, "LO_ACK");
            break; 
        }
    }
}

void session_init(const struct Message msg, struct Client* cli){
    // need to error check this (server ID already exists) severs full 
    struct Session* sess = malloc(sizeof(struct Session));
    strcpy(sess->sessionID,msg.data);
    sess->memberCount = 1; 
    sess->clients[0] = cli;
    
    for (int i = 0; i<MAX_SESSIONS; i++){ // currently just filling next available spot 
        if (sv->sessions[i] == NULL){
            sv->sessions[i] = sess;
            sess->sID = i;
            acknowledger(cli->connfd, "NS_ACK");
            break; 
        }
    }
}




