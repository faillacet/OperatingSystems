#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>

#define FRAMECOUNT 256
#define FRAMESIZE 256

char physicalMemory[FRAMECOUNT][FRAMESIZE];  // First is frame, second is offset

struct PageLookup {
    bool valid = false;
    short unsigned int physicalPage;
};

class PageTable {
private:
    FILE* fPtr;
    short unsigned int entries = 256;
    short unsigned int pageSize = 256; //bytes
    PageLookup* pageLookup;
    const char* backingFile = "BACKING_STORE.bin";

    void readPage(short unsigned int page);

public:
    PageTable();
    ~PageTable();
    short unsigned int getFrameNumber(short unsigned int pageNum);
    char getValueAtAddress(short unsigned int addr, short unsigned int offset);
};

PageTable::PageTable() {
    fPtr = fopen(backingFile, "r");
    pageLookup = new PageLookup[entries];
}

PageTable::~PageTable() {
    fclose(fPtr);
    delete[] pageLookup;
}

short unsigned int PageTable::getFrameNumber(short unsigned int pageNum) {
    if (pageLookup[pageNum].valid) {
        return pageLookup[pageNum].physicalPage;
    }
    else {  // Page Fault
        readPage(pageNum);
        pageLookup[pageNum].valid = true;
        pageLookup[pageNum].physicalPage = pageNum;
        return pageLookup[pageNum].physicalPage;
    }
}

void PageTable::readPage(short unsigned int page) {
    long offset = pageSize * page;
    fseek(fPtr, offset, SEEK_SET);
    fread(physicalMemory[page], sizeof(char), FRAMESIZE, fPtr);
}

char PageTable::getValueAtAddress(short unsigned int frame, short unsigned int offset) {
    return physicalMemory[frame][offset];
}

#endif // PAGETABLE_H