#include "server.h"


const char *clientIDs[] = {"wasif","nissar","instructor"}; // will need to code data structures 
const char *clientPasswords[] = {"legendary", "pathetic", "allknowing"}; // will need to code data structures 


int loginClient(char* credentials){
    // check client credentials against authorized credentials
    // check client credentials against real time connected users 
    unsigned int member = 1;
    char *username;   //member 1
    char *password;         //member 2

    int i1 = 0; //first index of a member
    int i2 = 0; //index of colon after the member 
    for (; (i2 < 2000) && (member < 3); i2++)
    {
        if (credentials[i2] == ',')
        {
            if (member == 1)    //total_frag
            {
                username = malloc(i2 - i1);
                memcpy(username, credentials + i1, i2 - i1);
            }
            else if (member == 2)    //size
            {
                password = malloc(i2 - i1);
                memcpy(password, credentials + i1, i2 - i1);
            }
            else{printf("error in credential parsing.\n"); exit(1);}

            i1 = i2 + 1;
            member++;
        }
    }

    printf("A client is trying to join with the credentals: \n username: %s\n password: %s\n", username, password);


    if (strcmp(clientIDs[0], username) == 0){
        if (strcmp(clientPasswords[0], password) == 0){
             // need to send acks with these prints as the data of the packet
             // need to keep track of this signed in user 
            printf("User has been authorized!\n");
        } else { 
            printf("Incorrect password for client!\n");// need to send nack
        }
    } else{
        printf("Client not found!\n"); // need to send nack 
    }
    free(username);
    free(password);
    return 1;
}

int joinSession(const struct Message msg){

}

int leaveSession(const struct Message msg){
    
}