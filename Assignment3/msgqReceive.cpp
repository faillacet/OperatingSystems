#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_LINE 80
#define ID 1234

struct msgBuff {
    long mtype = 0;
    char mtext[MAX_LINE];
};

void deleteQ(int id) {
    if (msgctl(id, IPC_RMID, 0) == -1) {
        perror("msgctl error: ");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    key_t  key;       /* message queue key */      
    msgbuf myMsg;
    int msgid;
    char msgCpy[MAX_LINE];

    key = ftok(argv[1], ID);   /* Create/attach message queue using key */      
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    // Attach MSG Q
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    while (true) {               
        // Recieve message from msg q  
        if (msgrcv(msgid, &myMsg, MAX_LINE, 0, 0) == -1) {
            if (errno != ENOMSG) {
                perror("msgrcv error:");
                exit(EXIT_FAILURE);
            }
        }
        else {
            // Prints Message
            strcpy(msgCpy, myMsg.mtext);
            cout << "Message recieved: " << msgCpy << endl;
        }

        // ends program in case type "exit" or "quit"
        if (!strcmp(msgCpy, "exit") || !strcmp(msgCpy, "quit")) {          
            // Delete msg q then exit
            deleteQ(msgid);
            cout << "Terminating message queue." << endl;
            cout << "Exiting program..." << endl;
            exit(EXIT_SUCCESS);
        }

        // Use system() to run recieved commands
        if (system(msgCpy) == -1) {
            perror("system error: ");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}