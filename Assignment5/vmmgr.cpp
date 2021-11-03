#include "FileHandler.h"
#include "PageTable.h"

#include <iomanip>
#include <bitset>

int main(int argc, char* argv[]) {
    if (argc != 2) { 
        cout << "Incorrect arguments..." << endl;
        exit(EXIT_FAILURE);
    }

    FileHandler addressHandler(argv[1]);
    PageTable pageTable;

    short unsigned int tempAddr;
    char memValue;
    for (int i = 0; i < addressHandler.getFileLength(); i++)
    {
        tempAddr = (addressHandler.logAddr[i].pageNumber << 8) + addressHandler.logAddr[i].pageOffset;
        cout << "Logical address: " << dec << tempAddr << endl;
       
        // Translate logical to physical
        tempAddr = pageTable.getFrameNumber(addressHandler.logAddr[i].pageNumber);
        tempAddr = (tempAddr << 8) + addressHandler.logAddr[i].pageOffset;
        cout << "Physical address: 0x" << hex << tempAddr << endl;

        // Get the value stored in mem at that location
        memValue = pageTable.getValueAtAddress((tempAddr >> 8), addressHandler.logAddr[i].pageOffset);
        cout << "Value at Physical Address: " << bitset<8>(memValue) << endl << endl;
    }
    
    // Display Page Faults via PageTable
    cout << "Amount of Page Faults: " << dec << pageTable.getPageFaultCount() << endl;
    cout << "Page Fault Rate: " <<  ((float)pageTable.getPageFaultCount() / (float)addressHandler.getFileLength()) * 100  << "%" << endl;
    
    exit(EXIT_SUCCESS);
}