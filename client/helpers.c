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
    read(connfd, ackReceipt, sizeof(ackReceipt));


    //whole receiving acks system needs to be written hard coding to LO_ACK 
    //to confirm login and try to join session with global username 
    if (strcmp(ackReceipt, "LO_ACK") == 0){
        printf("Logged in!\n");
        clientFD = connfd; 
        loginFlag = 1; 
        strcpy(username, source); 
        return 1;
    } else if (strcmp(ackReceipt, "LO_NACK") == 0) {
        printf("Log in failed.\n");
        return -1;
    } else if (strcmp(ackReceipt, "NS_ACK") == 0) {
        printf("New session successfully created!\n");
        return 1;
    } else if (strcmp(ackReceipt, "JS_ACK") == 0){
        printf("Session joined successfully!\n");
        return 1;
    } else if (strcmp(ackReceipt, "OUT_ACK") == 0){
        printf("Succesfully logged out of the server!");
        return 1; 
    }
    else {
        return 0;
    }
}

char* msgReader(struct Message msg){
    char* result = "some";
    return result;
}




