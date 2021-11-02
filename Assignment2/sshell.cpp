#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

#define MAX_LINE 80                             // Command can only contain up to 80 characters (79)
#define MAX_ARGS 20                             // (19) 20 Max arguments per command

void parseData(char* data, char** dest) {       // Changes one long string into array of command terminated by null pointer
    for (int i = 0; i < MAX_ARGS; i++)
    {
        dest[i] = strsep(&data, " ");

        if (dest[i] == NULL) {                  // Done if reach \n
            break;
        }
    }
}

void printArgs(char** arr) {                    // Prints arg arry (for testing purposes)
    int i = 0;
    while (arr[i] != NULL) {
        cout << arr[i] << " ";
        i++;
    }
    cout << endl;
}

bool checkWait(char** arr) {                    // checks if "&" was passed as an argument
    int i = 0;
    while (arr[i] != NULL) {
        string s(arr[i]);
        if (s == "&") {
            return true;
        }
        i++;
    }    
    return false;
}

void downShiftArgs(char** arr) {
    for (int i = 0; i < MAX_ARGS; i++) {
        arr[i] = arr[i+1];

        if (arr[i] == NULL) {                  // Done if reach \n
            break;
        }
    }
}

int main(void) {
    char rawInput[MAX_LINE];
    char* args[MAX_ARGS];  
    int pid;  

    while(true) {
        cout << "osh> ";
        cin.getline(rawInput, MAX_LINE);
        parseData(rawInput, args);

        // for testing purposes
        // printArgs(args);
        
        pid = fork();

        if (pid == -1) {                        // Fork Failed
            perror("fork failed");
            break;
        }
        else if (pid == 0) {                   // Child
            if (checkWait(args)) {
                downShiftArgs(args);
                int returnVal = execvp(args[0], args);
                return 0;
            }
            else {
                int returnVal = execvp(args[0], args);  // Attempts to execute the arguements, if not does nothing
                return 0;
            }
        }
        else {                                  // Parent
            if (checkWait(args)) {                             // If "&" command was entered, Parent will wait until child finishes
                int stat_val;
                pid_t child_pid;

                child_pid = wait(&stat_val);

                printf("Child has finished: PID = %d\n", child_pid);
                if (WIFEXITED(stat_val))
                    printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
                else
                    printf("Child terminated abnormally\n");
            }
        }

        if (!strcmp(rawInput, "exit") || !strcmp(rawInput, "quit")) {          // ends program in case type "exit" or "quit"
           break;
        }
    }
    cout << "Exiting Program..." << endl;
    return 0;
}