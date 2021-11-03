#include "FileHandler.h"
#include "PageTable.h"

#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc != 2) { 
        cout << "Incorrect arguments..." << endl;
        exit(EXIT_FAILURE);
    }

    FileHandler addressHandler(argv[1]);
    PageTable pageTable;

    short unsigned int temp;
    for (int i = 0; i < 1000; i++)
    {
        temp = (addressHandler.logAddr[i].pageNumber << 8) + addressHandler.logAddr[i].pageOffset;
        cout << "Logical address: " << hex << temp << endl;
       
        // Translate logical to physical
        temp = pageTable.getFrameNumber(addressHandler.logAddr[i].pageNumber);
        temp = (temp << 8) + addressHandler.logAddr[i].pageOffset;
        cout << "Physical address: " << hex << temp << endl;

        // Get the value stored in mem at that location
        
        
    }
    
  
    exit(EXIT_SUCCESS);
}