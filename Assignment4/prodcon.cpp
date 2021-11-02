#include "threadHandler.h"

int main(int argc, char* argv[]) {
    // Instantiate a threadHandler
    threadHandler handler(argc, argv);

    // Program execution is handled in this function
    handler.executeThreads();

    exit(EXIT_SUCCESS);
}