#include "client.h"

int establishConnection(char* clientID, char* password, char* serverIP, char* serverPort){
    printf("Attempting to establish connection...\n");
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    //socket() returns -1 on failure
    if (sock_fd == -1){
        printf("No connection\n");
        exit(1);
    }

    int sPort = atoi(serverPort);
    //declare sock struct
    struct sockaddr_in serv_addr;
    //make sure struct is empty
    memset(&serv_addr, 0, sizeof(serv_addr));
    //initialize the values in the sockaddr_in struct
    serv_addr.sin_family = AF_INET;
    //Finally, the sin_port must be in Network Byte Order (by using htons()!) (from Text)
    serv_addr.sin_port = htons(sPort);
    serv_addr.sin_addr.s_addr = inet_addr(serverIP);
    // dont need cause of first memset but just to be sure
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    if (connect(sock_fd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) { 
        printf("connection with the server failed.\n");
        return -1; 
    }
   
    char *loginData = malloc(sizeof(char) * 1100);
    asprintf(&loginData, "%s,%s", clientID, password);

    if(msgSender(1, strlen(loginData), clientID, loginData, sock_fd) == 1){
        return 1;
    } else {
        return -1;
    }

    return 1;
}

int msgSender (int type, unsigned int size, char* source, char * data, int connfd){
    char ackReceipt[100];
    char *serializedPacket = malloc(sizeof(char) * 1100);
    asprintf(&serializedPacket, "%d:%d:%s:%s", type,
        size, source, data);
    //printf("message to send: %s\n", serializedPacket);
    write(connfd, serializedPacket, strlen(serializedPacket));

    if (type == 1){
        read(connfd, ackReceipt, sizeof(ackReceipt));
        if (strcmp(ackReceipt, "LO_ACK") == 0){
            printf("Logged in!\n");
            clientFD = connfd; 
            loginFlag = 1; 
            strcpy(username, source);
            pthread_t t; 
            pthread_create(&t, NULL, messageListener, NULL); 
            return 1;
        } 
    }
    
    return 1;
}

char* msgReader(struct Message msg){
    char* result = "some";
    return result;
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




