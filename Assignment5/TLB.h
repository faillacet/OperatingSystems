#ifndef TLB_H
#define TLB_H

#include <stack>
using namespace std;

#define TLBSIZE 16

struct TLBLookup {
    short unsigned int pageNum;
    short unsigned int frameNum;
};

class TLB {
private:
    stack<TLBLookup> tblStack;
    TLBLookup lookup[TLBSIZE];
public:
    int isInTLB(short unsigned int page);

};

int TLB::isInTLB(short unsigned int page) {
    for (int i = 0; i < TLBSIZE; i++) {
        if (lookup[i].pageNum == page) {
            return i;
        }
    }
}

#endif //TLB_H