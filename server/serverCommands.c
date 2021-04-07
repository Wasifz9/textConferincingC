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

    for (int i = 0; i < MAX_CLIENTS; i++){
        if (sv->clients[i] != NULL){ 
            if (strcmp(sv->clients[i]->username, username) == 0){
                acknowledger(connfd, "LO_NACK", "This username is already logged in!");
                return;
            }
        }
    }

    //printf("A client is trying to join with the credentals: \n username: %s\n password: %s\n", username, password);

    for (int i = 0; i < ACCEPTED_CLIENTS; i++){
        if (strcmp(acceptedClientIDs[i], username) == 0){
            if (strcmp(acceptedClientPasswords[i], password) == 0){ // need to send the prints as LO_NAKS
                //printf("User has been authorized!\n");
                /// init user and add too server struct lists 
                // send appropriat LO_ACK
                
                client_init(msg, connfd);
                return;
            }else{
                //printf("Incorrect password for client!\n");
                acknowledger(connfd, "LO_NACK", "Incorrect password for client!");
                // SEND LO_NAK
                return;
            }
        }
    }
    //printf("User not found!\n");
    acknowledger(connfd, "LO_NACK", "This username isn't registered in the server!");
    free(username);
    free(password);
    
}

void inviteClient(const struct Message msg){
    unsigned int member = 1;
    char *recipientID;   //member 1
    char *sessionID;         //member 2

    int i1 = 0; //first index of a member
    int i2 = 0; //index of colon after the member

    for (; (i2 < 2000) && (member < 3); i2++)
    {
        if (msg.data[i2] == '|' || msg.data[i2] == '\0'){
            if (member == 1)    //total_frag
            {
                recipientID = malloc(i2 - i1);
                memcpy(recipientID, msg.data + i1, i2 - i1);
            }
            else if (member == 2)    //size
            {
                sessionID = malloc(i2 - i1);
                memcpy(sessionID, msg.data + i1, i2 - i1);
            }
            else{printf("error in credential parsing.\n"); exit(1);}

            i1 = i2 + 1;
            member++;
        }
    }

    printf("\nRecipient: %s, sessionID: %s\n", recipientID, sessionID);
    int cIndex = clientLookup(recipientID);
    int sIndex = sessionLookup(sessionID); 
    int srcIndex = clientLookup(msg.source); 
    
    if (cIndex == -1){
        acknowledger(sv->clients[srcIndex]->connfd, "N_INV", "Recipient not logged into the server!");
        return; 
    }

    if (sIndex == -1){
        acknowledger(sv->clients[srcIndex]->connfd, "N_INV", "Session doesn't exist!");
        return; 
    }

    if (sv->sessions[sIndex]->memberCount < MAX_SESSION_MEMS){
        acknowledger(sv->clients[srcIndex]->connfd, "A_INV", NULL); //send ack to sender 
        msgSender(10, strlen(sessionID), msg.source, sessionID, sv->clients[cIndex]->connfd); //send invitation to recipient
        return; 
    }

    acknowledger(sv->clients[srcIndex]->connfd, "N_INV", "Session is full!");

}


void createSession(const struct Message msg){
    int cIndex = clientLookup(msg.source);
    session_init(msg, sv->clients[cIndex]);
}

void joinSession(const struct Message msg){
    //struct Client* cli = clientLookup(sv, msg.source);
    int sIndex = sessionLookup(msg.data);
    int cIndex = clientLookup(msg.source);
    struct Client* cli = sv->clients[cIndex];

    //int scIndex = sessClientLookup(sv->sessions[sIndex], msg.source);
    if (sIndex == -1 || cIndex == -1){
        acknowledger(cli->connfd, "JS_NACK", "This session doesn't exist!"); //session doesnt exist
        return;  
    }

    for (int i = 0; i<MAX_SESSION_MEMS; i++){ // currently just filling next available spot 
        if (sv->sessions[sIndex]->clients[i] == NULL){
            sv->sessions[sIndex]->clients[i] = cli;
            sv->sessions[sIndex]->clients[i]->sessionJoined = sIndex;
            sv->clients[sIndex]->sessionJoined = sIndex;
            sv->clients[cIndex]->activeSessions+=1; 
            sv->sessions[sIndex]->memberCount+=1;
            acknowledger(cli->connfd, "JS_ACK", msg.data);
            return;  
        }
    }

    acknowledger(cli->connfd, "JS_NACK", "The session is full!"); //session full 
    return;
}

void leaveSession(const struct Message msg){
    int cIndex = clientLookup(msg.source);
    /*if (sv->clients[cIndex]->activeSessions == 1){
        printf("only in one session\n");
        int sIndex = sv->clients[cIndex]->sessionJoined;
        sv->clients[cIndex]->activeSessions-=1;

        int scIndex = sessClientLookup(sv->sessions[sIndex], msg.source);
    
        if (scIndex == -1){
            acknowledger(sv->clients[cIndex]->connfd, "LS_NACK", "You're not in this session!");
            return;
        }

        
        acknowledger(sv->sessions[sIndex]->clients[scIndex]->connfd, "LS_ACK", NULL);
        sv->sessions[sIndex]->clients[scIndex] = NULL;
        sv->sessions[sIndex]->memberCount--;
        
        if (sv->sessions[sIndex]->memberCount == 0){
            sv->sessions[sIndex] = NULL;
        }
        return; 
    }*/

    //sv->clients[cIndex]->sessionJoined=-1; // not in a session 
    int sIndex; 
    if (sv->clients[cIndex]->activeSessions == 1){
        sIndex = sv->clients[cIndex]->sessionJoined;
    } else {
        sIndex = sessionLookup(msg.data);
    }

    if (sIndex == -1){
        acknowledger(sv->clients[cIndex]->connfd, "LS_NACK", "Session doesn't exist!");
        return; 
    }
    
    int scIndex = sessClientLookup(sv->sessions[sIndex], msg.source);
    
    if (scIndex == -1){
        acknowledger(sv->clients[cIndex]->connfd, "LS_NACK", "You're not in this session!");
        return;
    }

    
    acknowledger(sv->sessions[sIndex]->clients[scIndex]->connfd, "LS_ACK", NULL);
    sv->sessions[sIndex]->clients[scIndex] = NULL;
    sv->sessions[sIndex]->memberCount--;
    

    sv->clients[cIndex]->activeSessions-=1;
    //setting session joined to being appropriate session location if only in one session
    if (sv->clients[cIndex]->activeSessions == 1){ 
        for (int i = 0;i<MAX_SESSIONS; i++){

            if (sv->sessions[i] != NULL){

                for (int j = 0;j<MAX_SESSION_MEMS; j++){
    
                    if (sv->sessions[i]->clients[j] != NULL){
           
                        if(strcmp(sv->sessions[i]->clients[j]->username,msg.source) == 0){
                            sv->clients[cIndex]->sessionJoined = i;

                        }
                    }
                } 
            }
        }
    }




    if (sv->sessions[sIndex]->memberCount == 0){
        sv->sessions[sIndex] = NULL;
    }
}

void logoutClient(const struct Message msg){
    //debugger(2);
    int cIndex = clientLookup(msg.source);
    if (cIndex == -1){
        printf("Client doesnt exist.\n");
        return;  
    }
    acknowledger(sv->clients[cIndex]->connfd, "OUT_ACK", NULL);  
    sv->clients[cIndex] = NULL;  
    //debugger(3);    

    for (int i = 0;i<MAX_SESSIONS; i++){
        //debugger(4);
        if (sv->sessions[i] != NULL){
            //debugger(5);
            for (int j = 0;j<MAX_SESSION_MEMS; j++){
                //debugger(6);
                if (sv->sessions[i]->clients[j] != NULL){
                    //debugger(7);
                    if(strcmp(sv->sessions[i]->clients[j]->username,msg.source) == 0){
                        //debugger(8);
                        sv->sessions[i]->memberCount--;
                        sv->sessions[i]->clients[j] = NULL; 
                    }
                }
            } 
        }
    }

}

void client_init(const struct Message msg, int connfd){ 
    // need to error check this (clients already logged in) cleints full
    struct Client* cli = malloc(sizeof(struct Client));
    strcpy(cli->username,msg.source);
    cli->sessionJoined = -1; //-1 means hasnt joined one yet 
    cli->connfd = connfd;
    cli->activeSessions = 0;
    for (int i = 0; i<ACCEPTED_CLIENTS; i++){ // currently just filling next available spot 
        if (sv->clients[i] == NULL){
            sv->clients[i] = cli;
            cli->cId = i;
            acknowledger(cli->connfd, "LO_ACK", "You've been authorized and logged in!");
            return; 
        }
    }

    acknowledger(cli->connfd, "LO_NACK", "Server is full!");
}

void session_init(const struct Message msg, struct Client* cli){
    // need to error check this (server ID already exists) severs full 
    struct Session* sess = malloc(sizeof(struct Session));
    strcpy(sess->sessionID,msg.data);
    sess->memberCount = 1; 
    sess->clients[0] = cli;

    for(int i = 1; i<MAX_SESSION_MEMS; i++){
        sess->clients[i] = NULL;
    }

    for (int i = 0; i<MAX_SESSIONS; i++){ // currently just filling next available spot 
        if (sv->sessions[i] == NULL){
            sv->sessions[i] = sess;
            sess->sID = i;
            cli->sessionJoined = i;
            cli->activeSessions +=1; 
            acknowledger(cli->connfd, "NS_ACK", NULL);
            return;
        }
    }
}

void groupMsg (const struct Message msg){
    int cIndex = clientLookup(msg.source);

    if (sv->clients[cIndex]-> activeSessions == 1){
        int sIndex = sv->clients[cIndex]->sessionJoined;        
       
        char* src = malloc(sizeof(char) * 100);
        asprintf(&src, "[%s]", sv->sessions[sIndex]->sessionID);
        char* data = malloc(sizeof(char) * 1100);
        asprintf(&data, "%s: %s", msg.source, msg.data);
        for (int j = 0;j<MAX_SESSION_MEMS; j++){
            if (sv->sessions[sIndex]->clients[j] != NULL && 
                strcmp(sv->sessions[sIndex]->clients[j]->username, msg.source) != 0)
            {
                msgSender(11, strlen(data), src, data, sv->sessions[sIndex]->clients[j]->connfd);
            }
        } 
    } else {          
        unsigned int member = 1;
        char *text = NULL;   //member 1
        char *sessionID = NULL;        //member 2

        int i1 = 0; //first index of a member
        int i2 = 0; //index of colon after the member

        for (; (i2 < 2000) && (member < 3); i2++)
        {
            if (msg.data[i2] == '@' || msg.data[i2] == '\0'){
                if (member == 1)    //total_frag
                {
                    text = malloc(i2 - i1);
                    memcpy(text, msg.data + i1, i2 - i1);
                }
                else if (member == 2)    //size
                {
                    sessionID = malloc(i2 - i1 - 1);
                    memcpy(sessionID, msg.data + i1, i2 - i1 -1);
                }
                else{printf("error in credential parsing.\n"); exit(1);}

                i1 = i2 + 1;
                member++;
            }
        }
        printf("sessionID in groupMsg: %s\n", sessionID);
        int sIndex = sessionLookup(sessionID);

        if (sIndex == -1){
            printf("Session doesnt exist!\n");
            return; 
        }
        
        int scIndex = sessClientLookup(sv->sessions[sIndex], msg.source);
        
        if (scIndex == -1){
            printf("You're not in this session!\n");
            return;
        }

        char* src = malloc(sizeof(char) * 100);
        asprintf(&src, "[%s]", sessionID);
        char* data = malloc(sizeof(char) * 1100);
        asprintf(&data, "%s: %s", msg.source, text);

        for (int j = 0;j<MAX_SESSION_MEMS; j++){
            if (sv->sessions[sIndex]->clients[j] != NULL && 
                strcmp(sv->sessions[sIndex]->clients[j]->username, msg.source) != 0)
            {
                msgSender(11, strlen(data), src, data, sv->sessions[sIndex]->clients[j]->connfd);
            }
        } 
    }
}

void listStatus(const struct Message msg){
        //Initialize Client List
    int MAX_LENGTH_CLIST = MAX_CLIENTS * MAX_CLIENT_ID * 2;
    char clist[MAX_LENGTH_CLIST];
    clist[0] = '\0';
    for (int i = 1; i < MAX_LENGTH_CLIST; i++)
        clist[i] = NULL;

    //Initialize Session List
    int MAX_LENGTH_SLIST = MAX_SESSIONS * MAX_SESSION_NAME * 2;
    char slist[MAX_LENGTH_SLIST];
    slist[0] = '\0';
    for (int i = 1; i < MAX_LENGTH_SLIST; i++)
        slist[i] = NULL;

    //Helper Strings
    char SPACE[2] = {' ', '\0'};
    char TAB[6] = {' ', ' ', ' ', ' ', ' ', '\0'};
    char CLIENTS[] = "Clients:  ";
    char SESSIONS[] = "Sessions:  ";
    char NO_CLIENTS[] = "No clients online";
    char NO_SESSIONS[] = "No sessions online";

    //Populate Client List
    strcat(clist, CLIENTS);
    int counter_c = 0;  //used to check if no clients
    for (int i = 0; i < MAX_CLIENTS ; i++){
        if(sv->clients[i] != NULL){
            strcat(clist, sv->clients[i]->username);
            strcat(clist, SPACE);
            counter_c++;
        }
    }
    if (counter_c == 0)
        strcat(clist, NO_CLIENTS);
    
    //Populate Session List
    strcat(slist, SESSIONS);
    int counter_s = 0;  //used to check if no sessions
    for (int i = 0; i < MAX_SESSIONS; i++){
        if(sv->sessions[i] != NULL){
            strcat(slist, sv->sessions[i]->sessionID);
            strcat(slist, SPACE);
            counter_s++;
        }
    }
    if (counter_s == 0)
        strcat(slist, NO_SESSIONS);

    //String to send over to client
    int MAX_LENGTH_LIST = MAX_LENGTH_CLIST + MAX_LENGTH_SLIST;
    char list[MAX_LENGTH_LIST];
    list[0] = '\0';
    for (int i = 1; i < MAX_LENGTH_LIST; i++)
        list[i] = NULL;
    strcat(list, clist);
    strcat(list, TAB);
    strcat(list, slist);

    //For debugging server-side
    //printf("clients: %s\n", clist);
    //printf("sessions: %s\n", slist);
    int cIndex = clientLookup(msg.source);
    acknowledger(sv->clients[cIndex]->connfd, "QU_ACK", list);
    //printf("overall: %s\n", list);

    return;
}