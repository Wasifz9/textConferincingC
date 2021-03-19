#include "server.h"

int eventHandler (struct Server* sv, int connfd){ 
    //while(1){
 
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
            loginClient(sv, msg, connfd);
        } else if (msg.type == 2){
            createSession(sv, msg);
        } else if (msg.type == 3){
            leaveSession(sv, msg);
        } 

        // testing the creation of clients and sessions
        for (int i = 0; sv->clients[i] != NULL; i++){
            printf("%s is Client #%d in the TCserver!\n", sv->clients[i]->username, sv->clients[i]->cId);
        } 
        //
        //session_init(sv,msg, sv->clients[0]);
        //printf("%s has been initialized as a session!\n", sv->sessions[0]->sessionID);

        // writing an ack here for now. but response handler should call a response function that sends
        // the required ack packets as described in the document 
        
        
    //}
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



struct Client* clientLookup(struct Server* sv, char* username){ // hash later if we're nasty 
    for (int i = 0; sv->clients[i] != NULL; i++){
        if (strcmp(sv->clients[i]->username, username) == 0){
            return sv->clients[i];
        }
    } 
}

struct Session* sessionLookup(struct Server* sv, char* sessionID){
    return; 
}

void debugger(int code){
    printf("you made it here: %d\n", code);
}

void acknowledger(int connfd, char* ackToSend){
    write(connfd, ackToSend, strlen(ackToSend));
}