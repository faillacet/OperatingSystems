#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_LINE 80
#define ID 1234

struct msgBuff {
    long mtype = 1;
    char mtext[MAX_LINE];
};

int main(int argc, char *argv[]) {
    key_t  key;       /* message queue key */      
    msgbuf myMsg;
    char msgCpy[MAX_LINE];
    int msgid;

    key = ftok(argv[1], ID);   /* Create/attach message queue using key */      
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    // Create MSG Q
    msgid = msgget(key, 0666 | IPC_CREAT);

    while (true) {               
        cout << "msgq> ";           
        cin.getline(msgCpy, MAX_LINE);
        strcpy(myMsg.mtext, msgCpy);

        // read command from stdin then send command to message queue  
        if (msgsnd(msgid, &myMsg, MAX_LINE, 0)) {
            perror("msgsnd error");
            exit(EXIT_FAILURE);
        }   
        cout << "Message sent: " << myMsg.mtext << endl;

        // ends program in case type "exit" or "quit"
        if (!strcmp(msgCpy, "exit") || !strcmp(msgCpy, "quit")) {          
            // Delete msg q then exit
            break;
        }
    }
    cout << "Exiting program..." << endl;
    return 0;
}