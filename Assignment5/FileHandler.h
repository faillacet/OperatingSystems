#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <iostream>
using namespace std;

struct LogicalAddress {
    short unsigned int pageNumber;
    short unsigned int pageOffset;
};

class FileHandler {
private:
    ifstream inFile;
    int fileLength;

    int getFileLength();
    void copyFile();

public:
    LogicalAddress* logAddr;

    FileHandler();
    FileHandler(char* file);
    ~FileHandler();
};

FileHandler::FileHandler(char* file) {
    inFile.open(file);
    fileLength = getFileLength();
    logAddr = new LogicalAddress[fileLength];

    copyFile();

}

FileHandler::~FileHandler() {
    delete[] logAddr;
    inFile.close();
}

int FileHandler::getFileLength() {
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

void FileHandler::copyFile() {
    int x;
    int iterator = 0;
    while (inFile >> x) {
        logAddr[iterator].pageNumber = (x & 0x0000FF00) >> 8;
        logAddr[iterator].pageOffset = (x & 0x000000FF);
        iterator++;
    }
}

#endif // FILEHANDLER_H