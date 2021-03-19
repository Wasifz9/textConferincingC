#include "server.h"

int eventHandler (int *conn_fd){
    int connfd = *conn_fd;
    while(1){
        printf("\nWaiting for new read in eventHandler.. \n");
        memset(&buff, 0, sizeof(buff)); 
        read(connfd, buff, sizeof(buff));
    
        // error handle the processing. 
        // currently thinking to do most error handling on client side (size length etc)
        // so we dont have to send too many nacks. nacks only if the networks part doesnt work for
        // some reason 
        struct Message msg;
        processPacket(buff, &msg); 
        
        printf("\n-- Message --\n");
        printf("type = %d | size = %d | sourceID = %s\n",
        msg.type, msg.size, msg.source);
        printf("data: %s\n", msg.data);
        
        
        if (msg.type == 1){ // can set up the types to corresoond to certain numbers in header file
            loginClient(msg, connfd);
        } else if (msg.type == 2){
            createSession(msg);
        } else if (msg.type == 3){
            leaveSession(msg);
        } 

        // testing the creation of clients and sessions
        for (int i = 0; i < MAX_CLIENTS ; i++){
            if(sv->clients[i] != NULL){
                printf("%s is Client #%d in the TCserver!\n", sv->clients[i]->username, sv->clients[i]->cId);
            }
        } 
        
        for (int i = 0;i<MAX_SESSIONS; i++){
            if (sv->sessions[i] != NULL){
                printf("%s is session #%d in the TCserver!\n", sv->sessions[i]->sessionID, sv->sessions[i]->sID);
                printf("Active members: \n");
                for (int j = 0;j<MAX_SESSION_MEMS; j++){
                    if(sv->sessions[i]->clients[j] != NULL){
                        printf("%s\n", sv->sessions[i]->clients[j]->username);
                    }
                } 
            }
        } 

        //
        //session_init(sv,msg, sv->clients[0]);
        //printf("%s has been initialized as a session!\n", sv->sessions[0]->sessionID);

        // writing an ack here for now. but response handler should call a response function that sends
        // the required ack packets as described in the document 
        
        
    }
}

// parse paclet into members and data 
void processPacket(char* packet, struct Message* msg){
    unsigned int member = 1;
    char *type;   //member 1
    char *size;         //member 2
    char *source;      //member 3

    int i1 = 0; //first index of a member
    int i2 = 0; //index of colon after the member 

    for (; (i2 < 2000) && (member < 4); i2++)
    {
        if (packet[i2] == ':')
        {
            if (member == 1)    //total_frag
            {
                type = malloc(i2 - i1);
                memcpy(type, packet + i1, i2 - i1);
            }
            else if (member == 2)    //size
            {
                size = malloc(i2 - i1);
                memcpy(size, packet + i1, i2 - i1);
            }
            else if (member == 3)    //size
            {
                source = malloc(i2 - i1);
                memcpy(source, packet + i1, i2 - i1);
            }
            else{printf("ERROR IN PROCESSPACKET.\n"); exit(1);}

            i1 = i2 + 1;
            member++;
        }
    }


    
    memset(&msg->data, 0, atoi(size)+1);
    memcpy(&msg->data, packet + i1, atoi(size));
    msg->type = atoi(type);
    msg->size = atoi(size);
    strcpy(msg->source,source); 
    
    /// based on type, we process data portion differently 
    free(type);
    free(source);
    free(size);
}



int clientLookup(char* username){ // hash later if we're nasty 
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (sv->clients[i] != NULL){
            if (strcmp(sv->clients[i]->username, username) == 0){
                //return sv->clients[i];
                return i;
            }
        }
    }
    return -1; 
}

int sessClientLookup(struct Session* sess, char* username){ // hash later if we're nasty 
    for (int i = 0; i < MAX_SESSION_MEMS; i++){
        if (sess->clients[i] != NULL){
            if (strcmp(sess->clients[i]->username, username) == 0){
                //return sess->clients[i];
                return i;
            }
        }
    }
    return -1;
}

int sessionLookup(char* sessionID){
    for (int i = 0; i < MAX_SESSIONS; i++){
        if (sv->sessions[i] != NULL){
            if (strcmp(sv->sessions[i]->sessionID, sessionID) == 0){
                //return sv->sessions[i];
                return i;
            }
        }
    }
    return -1;  
}

void debugger(int code){
    printf("you made it here: %d\n", code);
}

void acknowledger(int connfd, char* ackToSend){
    write(connfd, ackToSend, strlen(ackToSend));
}