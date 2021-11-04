#ifndef TLB_H
#define TLB_H

#include <deque>
using namespace std;

#define TLBSIZE 16

struct TLBLookup {
    short unsigned int pageNum;
    short unsigned int frameNum;

    bool operator==(short unsigned int page) {
        if (page == this->pageNum) {
            return true;
        }
        return false;
    }
};

class TLB {
private:
    deque<TLBLookup> pageLookup;
    int currentIndex;

public:
    int hitCount = 0;
    int missCount = 0;

    bool isInTLB(short unsigned int page);
    short unsigned int getPhysicalAddr(short unsigned int offset);
    void addElement(short unsigned int physAddr);
};

bool TLB::isInTLB(short unsigned int page) {
    for (int i = 0; i < pageLookup.size(); i++)
    {
        if (pageLookup.at(i) == page) {
            currentIndex = i;
            hitCount++;
            return true;
        }
    }
    missCount++;
    return false;
}

short unsigned int TLB::getPhysicalAddr(short unsigned int offset) {
    TLBLookup lookup = pageLookup.at(currentIndex);
    return ((lookup.frameNum << 8) + offset);
}

void TLB::addElement(short unsigned int page) {
    TLBLookup temp;
    temp.pageNum = page;
    temp.frameNum = page;
    if (pageLookup.size() == 16) {
        pageLookup.pop_front();
        pageLookup.push_back(temp); 
    }
    else {
        pageLookup.push_back(temp);
    }
}

#endif //TLB_H