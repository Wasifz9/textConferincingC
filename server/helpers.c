#include "server.h"

void* eventHandler (int *conn_fd){
    int connfd = *conn_fd;
    while(1){
        //printf("\nWaiting for new read in eventHandler.. \n");
        memset(&buff, 0, sizeof(buff)); 
        read(connfd, buff, sizeof(buff));
    
        // error handle the processing. 
        // currently thinking to do most error handling on client side (size length etc)
        // so we dont have to send too many nacks. nacks only if the networks part doesnt work for
        // some reason 

        struct Message msg;
        processPacket(buff, &msg); 
        
        printf("\n|||||||| Message Received ||||||||\n\n");
        //printf("Raw data: %s\n", buff);
        printf("type = %d | size = %d | sourceID = %s\n",
        msg.type, msg.size, msg.source);
        printf("data: %s\n", msg.data);
        
        printf("\n--- Server Output ---\n");
        printf("%s: ", msg.source);
        if (msg.type == 1){ // can set up the types to corresoond to certain numbers in header file
            loginClient(msg, connfd);
        } else if (msg.type == 2){
            createSession(msg);
        } else if (msg.type == 3){
            leaveSession(msg);
        } else if (msg.type == 4){
            joinSession(msg);
        } else if (msg.type == 8){
            listStatus(msg);   
        } else if (msg.type == 6){
            logoutClient(msg);
        } else if (msg.type == 7){
            groupMsg(msg);
            printf("Sending text -> %s\n", msg.data);
        } else if (msg.type == 0){
            inviteClient(msg);
        }

        printf("\n-------- Server Status --------\n\n");
        printf("- Client List -\n");

        // testing the creation of clients and sessions
        for (int i = 0; i < MAX_CLIENTS ; i++){
            if(sv->clients[i] != NULL){
                printf("%s is Client #%d in the TCserver and has connfd: %d!\n", 
                    sv->clients[i]->username, sv->clients[i]->cId, sv->clients[i]->connfd);
            }
        } 

        printf("\n- Session List -\n");

        for (int i = 0;i<MAX_SESSIONS; i++){
            if (sv->sessions[i] != NULL){
                if (sv->sessions[i]->memberCount == 0){
                    sv->sessions[i] = NULL;
                    continue;
                }
                printf("%s is session #%d in the TCserver!\n", sv->sessions[i]->sessionID, sv->sessions[i]->sID);
                printf("Active members: \n");
                for (int j = 0;j<MAX_SESSION_MEMS; j++){
                    if(sv->sessions[i]->clients[j] != NULL){
                        printf("%s\n", sv->sessions[i]->clients[j]->username);
                    }
                }
                printf("\n");
            }
        } 

        printf("|||||||||||||||||||||||||||||||||\n\n");

        if (msg.type == 6){
            return;
        }

        
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
    if (msg->type > 9){ 
        msg->type=msg->type/10; 
    }
    strcpy(msg->source,source); 
    
    /// based on type, we process data portion differently 
    free(type);
    free(source);
    free(size);
}



int clientLookup(char* username){ // hash later if we're nasty 
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (sv->clients[i] != NULL){
            //debugger(123);
            if (strcmp(sv->clients[i]->username, username) == 0){
                //debugger(234);
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
            printf("%s,%s\n", sessionID, sv->sessions[i]->sessionID);
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

void acknowledger(int connfd, char* ackToSend, char* error){
    int type;
    int size = 0;

    if (strcmp(ackToSend, "LO_ACK") == 0) { // 1
        type = 1;
        size = strlen(error);
        printf("Logged in!\n");
    } 
    if (strcmp(ackToSend, "LO_NACK") == 0) { // 2
        printf("Couldn't login: %s\n", error);
        type = 2;
        size = strlen(error); 
    } else if (strcmp(ackToSend, "NS_ACK") == 0) { // 3 
        printf("Session created!\n");
        type = 3;
    } else if (strcmp(ackToSend, "JS_ACK") == 0){ // 4
        printf("Joined session!\n");
        size = strlen(error);
        type = 4; 
    } else if (strcmp(ackToSend, "JS_NACK") == 0){ // 5 
        printf("Couldn't join session: %s\n", error);
        type = 5;
        size = strlen(error); 
    } else if (strcmp(ackToSend, "LS_ACK") == 0){ // 6
        printf("Left session!\n");
        type = 6;
    } else if (strcmp(ackToSend, "OUT_ACK") == 0){ // 7
        printf("Logging out!\n");
        type = 7;
    } else if (strcmp(ackToSend, "TXT_ACK") == 0){ // not doing anything rn
        printf("Text sent\n!");
    } else if (strcmp(ackToSend, "QU_ACK") == 0){
        printf("Sending server status!\n");
        type = 8; 
        size = strlen(error);
    } else if (strcmp(ackToSend, "N_INV") == 0){
        printf("Invitation failed!\n");
        type = 0; 
        size = strlen(error);  
    } else if (strcmp(ackToSend, "A_INV") == 0){
        printf("Sending invitation!\n");
        type = 9; 
    } else if (strcmp(ackToSend, "LS_NACK") == 0){
        printf("Leaving session failed!\n");
        type = 12; 
        size = strlen(error);
    } 

    msgSender (type, size,"TCServer", error, connfd);
    //write(connfd, ackToSend, strlen(ackToSend));
}


int msgSender (int type, unsigned int size, char* source, char * data, int connfd){
    char ackReceipt[100];
    char *serializedPacket = malloc(sizeof(char) * 1100);
    asprintf(&serializedPacket, "%d:%d:%s:%s", type,
        size, source, data);
    //printf("message to send: %s\n", serializedPacket);
    write(connfd, serializedPacket, strlen(serializedPacket));
}