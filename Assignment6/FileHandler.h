#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <iostream>
using namespace std;

class FileHandler {
private:
    ifstream inFile;
    int fileLength;

    int calcFileLength();
    void copyFile(int arr[]);

public:
    FileHandler();
    FileHandler(char* file, int arr[]);
    ~FileHandler();
};

FileHandler::FileHandler(char* file, int arr[]) {
    inFile.open(file);
    if (inFile.fail()) {
        perror("Unable to open file...");
        exit(EXIT_FAILURE);
    }
    fileLength = calcFileLength();
    copyFile(arr);
}

FileHandler::~FileHandler() {
    inFile.close();
}

int FileHandler::calcFileLength() {
    string s;
    int count = 0;
    while(!inFile.eof()) {
        getline(inFile, s);
        if (s != "") {
            count++;
        }
    }
    inFile.clear();
    inFile.seekg(0);
    return count;
}

void FileHandler::copyFile(int arr[]) {
    int x;
    int iterator = 0;
    while (inFile >> x) {
        arr[iterator] = x;
        arr[iterator] = x;
        iterator++;
    }
}

#endif // FILEHANDLER_H