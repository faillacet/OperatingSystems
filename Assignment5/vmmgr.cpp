#include "FileHandler.h"
#include "PageTable.h"
#include "TLB.h"

#include <iomanip>
#include <bitset>

int main(int argc, char* argv[]) {
    if (argc != 2) { 
        cout << "Incorrect arguments..." << endl;
        exit(EXIT_FAILURE);
    }

    FileHandler addressHandler(argv[1]);
    PageTable pageTable;
    TLB tlb;

    short unsigned int tempAddr;
    char memValue;
    for (int i = 0; i < addressHandler.getFileLength(); i++)
    {
        tempAddr = (addressHandler.logAddr[i].pageNumber << 8) + addressHandler.logAddr[i].pageOffset;
        cout << "Logical address: " << dec << tempAddr << endl;
       
        // Check if it is in TLB
        // Case is in table
        if (tlb.isInTLB(addressHandler.logAddr[i].pageNumber)) {
            // Get physical address via TBL
            tempAddr = tlb.getPhysicalAddr(addressHandler.logAddr[i].pageOffset);
            cout << "Physical address: 0x" << hex << tempAddr << endl;

            // Get value stored in mem at that location
            memValue = physicalMemory[addressHandler.logAddr[i].pageNumber][addressHandler.logAddr[i].pageOffset];
            cout << "Value at Physical Address: " << bitset<8>(memValue) << endl << endl;
        }
        else {
            // Translate logical to physical via PageTable - loads into mem if not there
            tempAddr = pageTable.getFrameNumber(addressHandler.logAddr[i].pageNumber);
            tempAddr = (tempAddr << 8) + addressHandler.logAddr[i].pageOffset;
            cout << "Physical address: 0x" << hex << tempAddr << endl;

            // Get the value stored in mem at that location
            memValue = pageTable.getValueAtAddress((tempAddr >> 8), addressHandler.logAddr[i].pageOffset);
            cout << "Value at Physical Address in Binary: " << bitset<8>(memValue) << endl;
            cout << "Value at Physical Address as Char: " << (char)memValue << endl << endl;

            // Push to TLB
            tlb.addElement(addressHandler.logAddr[i].pageNumber);
        }
    }
    
    // Display Page Faults
    cout << "Amount of Page Faults: " << dec << pageTable.getPageFaultCount() << endl;
    cout << "Page Fault Rate: " <<  ((float)pageTable.getPageFaultCount() / (float)addressHandler.getFileLength()) * 100  << "%" << endl << endl;
    
    // Display TBL hit rate
    cout << "TBL algorithm used: FIFO" << endl;
    cout << "Amount of TLB hits: " << tlb.hitCount << endl;
    cout << "TBL Hit rate: " << ((float)tlb.hitCount / (float)(tlb.hitCount+tlb.missCount)) * 100 << "%" << endl;

    exit(EXIT_SUCCESS);
}