#include <stdio.h>
#include <unistd.h>
#include <iostream>

#define NRECORDS 100
struct RECORD {
    int size;
    char str[24];
};

class FileHandler {
public:
    int recordToChange;
    int recordChangeTo;
    int size[NRECORDS];
    RECORD records[NRECORDS];
    
    bool getInputFromCMDLine(int a, int b);
    bool getInputFromProgram();
    void initializeRecords();
    void writeToFile();
    void changeFile();
    void writeToConsole();

    FileHandler();
};

// Handles commands passed from command line
// a is cmd 1, b is cmd 2
// Returns true if invalid entry
bool FileHandler::getInputFromCMDLine(int a, int b) {
    recordToChange = a;
    recordChangeTo = b;
    if (!(recordToChange > 0 && recordToChange <= 100)) {
        printf("Not a valid entry, exiting program...\n");
        return true;
    }
    return false;
}

// Used if commands are not passed through command line
// Returns true if invalid entry
bool FileHandler::getInputFromProgram() {
    char buff[24];

    printf("Please enter the Record # to change:\n");
    int a = read(STDIN_FILENO, buff, sizeof(buff));
    recordToChange = atoi(buff);

    printf("Please enter value you wish to change to:\n");
    int b = read(STDIN_FILENO, buff, sizeof(buff));
    recordChangeTo = atoi(buff);

    if (a == -1 || b == -1)
        throw "read";

    if (!(recordToChange > 0 && recordToChange <= 100)) {
        printf("Not a valid entry, exiting program...\n");
        return true;
    }
    return false;
}

// Populate Array of Records w/ size and initalized correct string
void FileHandler::initializeRecords() {
    for (int i = 0; i < NRECORDS; i++) {
        records[i].size = sprintf(records[i].str, "RECORD-%d\n", i+1);
    }
}

// Using fwrite (void *ptr, size_t size, size_t nmemb, FIle *stream)
// To fill file with records data
void FileHandler::writeToFile() {
    FILE *fPtr = fopen("records.txt", "w+");
    for (int i = 0; i < NRECORDS; i++) {
        fwrite(records[i].str, records[i].size, 1, fPtr);
    }
    fclose(fPtr);
}

//  Handles changing the value specified by the user
void FileHandler::changeFile() {
    records[recordToChange-1].size = sprintf(records[recordToChange-1].str, "RECORD-%d\n", recordChangeTo);

    int returnVal, offset = 0;
    FILE *fPtr = fopen("records.txt", "r+");

    for (int i = 0; i < recordToChange - 1; i++) {                // Calculating the byte offset needed to find the line that we are changing 
        offset += records[i].size;
    }

    returnVal = fseek(fPtr, offset, SEEK_SET);                    // fSeek uses SEEK_SET to use begining of file as ref point
    if (returnVal == -1)
        throw "fseek";

    for (int i = recordToChange - 1; i < NRECORDS; i++) {         // fwrite changes the value
        fwrite(records[i].str, records[i].size, 1, fPtr);         // Sometimes formatting messes up file, easiest solution is to rewrite everything
    }
    fclose(fPtr);
}

// Use write() to write contents of file to console
void FileHandler::writeToConsole() {
    for (int i = 0; i < NRECORDS; i++) {
        if (write(STDOUT_FILENO, records[i].str, records[i].size) == -1)
            throw "write";
    }
}

// Default Constructor Automatically wipes/creates new file and fills with records
FileHandler::FileHandler() {
    initializeRecords();
    writeToFile();
}

int main(int argc, char *argv[]) {
    FileHandler filehandler;

    // Argument handling
    for (int arg = 0; arg < argc; arg++) {
        if (argv[arg][0] == '-')
            printf("option: %s\n", argv[arg]+1);
        else
            printf("argument %d: %s\n", arg, argv[arg]);
    }

    // If we use command line to use program:
    // Use arg[1] as Record To Change
    // Use arg[2] as New Record Value    

    try {
        if (argv[1] != nullptr) { 
            if (filehandler.getInputFromCMDLine(atoi(argv[1]), atoi (argv[2]))) {
                return 0;
            }
        }
        else {  // Prompt user to get input using read()
            if (filehandler.getInputFromProgram()) {
                return 0; 
            }
        }

        filehandler.changeFile();
        filehandler.writeToConsole();
    }
    catch (const char * txtExp) {
        printf("Exception: %s", txtExp);
    }  

    return 0;
}