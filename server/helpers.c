#include "server.h"

int processPacket(char* packet){
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


    struct Message msg; 
    memcpy(&msg.data, packet + i1, atoi(size));
    msg.type = atoi(type);
    msg.size = atoi(size);
    strcpy(msg.source,source); 

    /// based on type, we process data portion differently 
    int ret = responseHandler(msg);
    free(type);
    free(source);
    free(size);

}


/// this function will call the right function from server commands
/// and return the correct corresponding ACK/NACK depending on the 
/// return value of the server command 


int responseHandler(struct Message msg){
    /*printf("\n-- Message --\n");
    printf("type = %d | size = %d | sourceID = %s\n",
        msg.type, msg.size, msg.source);
    printf("data: %s\n", msg.data);*/

    if (msg.type == 1){
        int ret = loginClient(msg.data);
    }

}

void debugger(int code){
    printf("you made it here: %d\n", code);
}