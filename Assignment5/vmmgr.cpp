#include "FileHandler.h"

int main(int argc, char* argv[]) {
    if (argc != 2) { 
        cout << "Incorrect arguments..." << endl;
        exit(EXIT_FAILURE);
    }

    FileHandler fileHandler(argv[1]);

    exit(EXIT_SUCCESS);
}