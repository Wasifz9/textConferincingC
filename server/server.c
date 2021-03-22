#include "server.h"

// have to write more code to handle requests cleaner. 
// this function will be complciated 



int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Try again with 1 argument.\n");
        exit(0);
    }
    //INET makes it IPv4 and STREAM chooses TCP
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    //socket() returns -1 on failure 
    if (sock_fd == -1){
        printf("Sock Error!\n");
        exit(1);
    }
    //convert argument from string to integer and store
    int listen_port = atoi(argv[1]);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); //make sure struct is empty
    //initialize the values in the sockaddr_in struct
    serv_addr.sin_family = AF_INET; 
    //Finally, the sin_port must be in Network Byte Order (by using htons()!) (from Text)
    serv_addr.sin_port = htons(listen_port);
    //then ina.sin_addr.s_addr references the 4-byte IP address (in Network Byte Order) (from Text)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	 
    // dont need cause of first memset but just to be sure
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero)); 

    // attempting a bind 
    if (bind(sock_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1 ){
        printf("couldn't bind.\n");
        exit(1);
    }

    sv = server_init();


    if ((listen(sock_fd, 10)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else{
        printf("Server listening..\n"); 
    }

    int connfd; 
        
    while(1){
        // declare and get length of client_addr
        struct sockaddr_in client_addr;
        int cli_length = sizeof(client_addr);

        int connfd = accept(sock_fd, (struct sockaddr *)&client_addr, &cli_length); 

        if (connfd < 0){
            printf("couldn't accept.\n");
            exit(1);
        } else {
            pthread_t t; 
            int *cli_connfd = malloc(sizeof(int));
            *cli_connfd = connfd;   
            pthread_create(&t, NULL, eventHandler, cli_connfd);
        }
    }

    close(connfd);
    close(sock_fd);
}


struct Server* server_init(){
	struct Server *sv;
	sv = malloc(sizeof(struct Server));
    sv->activeSessions = 0;
    sv->activeClients = 0;
    
    for(int i = 0; i < MAX_SESSIONS ; ++i)
        sv->sessions[i] = NULL;
    for(int i = 0; i < MAX_CLIENTS; ++i)
        sv->clients[i] = NULL;
    
    return sv;
}



