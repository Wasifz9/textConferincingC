#include "server.h"

int eventHandler (int connfd){ 
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
            int ret = loginClient(msg);
        } else if (msg.type == 2){
            int ret = joinSession(msg);
        } else if (msg.type == 3){
            int ret = leaveSession(msg);
        }


        // writing an ack here for now. but response handler should call a response function that sends
        // the required ack packets as described in the document 
        write(connfd, "ACK", sizeof(3));
    //}
}







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


/// this function will call the right function from server commands
/// and return the correct corresponding ACK/NACK depending on the 
/// return value of the server command 


void debugger(int code){
    printf("you made it here: %d\n", code);
}