#include "FileHandler.h"
#include "PageTable.h"

int main(int argc, char* argv[]) {
    if (argc != 2) { 
        cout << "Incorrect arguments..." << endl;
        exit(EXIT_FAILURE);
    }

    FileHandler addressHandler(argv[1]);
    PageTable pageTable;
  
    exit(EXIT_SUCCESS);
}