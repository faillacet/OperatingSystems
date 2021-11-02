#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>

struct PageLookup {
    bool valid = 0;
    short unsigned int physicalPage;
};

class PageTable {
private:
    FILE* fPtr;
    short unsigned int entries = 256;
    short unsigned int pageSize = 256; //bytes
    PageLookup* pageLookup;
    const char* backingFile = "BACKING_STORE.bin";
public:
    PageTable();
    ~PageTable();
    short unsigned int getFrameNumber(short unsigned int pageNum);
};

PageTable::PageTable() {
    fPtr = fopen(backingFile, "r");
    pageLookup = new PageLookup[entries];
}

PageTable::~PageTable() {
    delete[] pageLookup;
}

short unsigned int PageTable::getFrameNumber(short unsigned int pageNum) {
    if (pageLookup[pageNum].valid) {
        return pageLookup[pageNum].physicalPage;
    }
    else {  // Page Fault
        // Do the stuff the assignment says
    }
}

#endif // PAGETABLE_H