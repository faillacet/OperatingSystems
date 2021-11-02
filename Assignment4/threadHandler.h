#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include "checksum.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
using namespace std;

#define MEMID 1
#define BUFSIZE 32

// Global variables
sem_t semFull, semEmpty;
pthread_mutex_t mutex;

char* shmBuf;                    // shmMem buffer
int nitems;                      // Num of items in the bounded buffer
int shmID;                       // shared mem ID
int ntimes;                             // Num of times to read/write from shared buffer

class threadHandler {
private:
    char* keyfile;                          // Filename used to gen shared memkey
    char defaultFile[10] = "ftok.file";    

    key_t key;                              // ftok key
    pthread_t tid[2];                       // Thread ids
    void* result;
public:
    threadHandler(int argc, char* argv[]);  // Processes Arguments from command line -- constructor step
    ~threadHandler();                       // Destructor

    void executeThreads();                  // Runs the program once based on user specs
    void generateSharedMem();               // Creates shared memory used in thread communication
    static void deleteSharedMem();          // Terminate shared memory
    static void initializeSemaphores();     // Creates semaphors
    static void deleteSemaphores();         // Removes the semaphors
    static void exitOnFailure();            // Cleanup if program terminates in abnormal way
    
    static void *producerThread(void *arg); // Producer Program executed when thread is created
    static void *consumerThread(void *arg); // Consumer Program executed when thread is created
};

threadHandler::threadHandler(int argc, char* argv[]) {
    if (argc == 4) {                // Case File Name is given
        keyfile = argv[1];
        nitems = stoi(argv[2]);
        ntimes = stoi(argv[3]);
    }
    else if (argc == 3) {           // NO file name given, use default
        keyfile = defaultFile;
        nitems = stoi(argv[1]);
        ntimes = stoi(argv[2]);
    }
    else {
        cout << "Incorrect num of parameters, try again..." << endl;
        exit(EXIT_FAILURE);
    }

    // Check For Preconditions
    if (nitems <= 0 || nitems >= 1024) {
        cout << "nitmes constraint failure" << endl;
        exit(EXIT_FAILURE);
    }
    if (ntimes <= 0) {
        cout << "ntimes constraint failure" << endl;
        exit(EXIT_FAILURE);
    }

    // Random seed for rand func in producer
    srand (time(NULL));
}

threadHandler::~threadHandler() {

}

void threadHandler::executeThreads() {
    initializeSemaphores();
    generateSharedMem();

    // Create Producer Thread
    if (pthread_create(&tid[0], NULL, producerThread, NULL) != 0) {
        perror("Thread create error");
        exit(EXIT_FAILURE);
    }

    // Create Consumer Thread
    if (pthread_create(&tid[1], NULL, consumerThread, NULL) != 0) {
        perror("Thread create error");
        exit(EXIT_FAILURE);
    }

    // Remove Producer Thread
    if (pthread_join(tid[0], &result) != 0) {
        perror("Thread join error");
		exit(EXIT_FAILURE);
    }

    // Remove Consumer Thread
    if (pthread_join(tid[1], &result) != 0) {
        perror("Thread join error");
		exit(EXIT_FAILURE);
    }

    deleteSharedMem();
    deleteSemaphores();

    cout << "Program ran " << ntimes << " times, reading/writing " << nitems << " buffers each time!" << endl;
    cout << "Now exiting..." << endl;
}

void threadHandler::generateSharedMem() {
    // Create Unique Key 
    key = ftok(keyfile, MEMID);      
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    // Use key to create shared memory of needed size
    shmID = shmget(key, BUFSIZE * nitems, IPC_CREAT | 0666);
    if (shmID < 0) {
        perror("shmget error");
        exit(EXIT_FAILURE);
    }

    // Attach to shared mem and give location to both threads
    shmBuf = (char*)shmat(shmID, NULL, 0);
    if (shmBuf == (void*) -1) {
        perror("Failed to attach to mem (producer)");
        exit(EXIT_FAILURE);
    }
}

void threadHandler::deleteSharedMem() {
    // Delete Shared Mem
    shmctl(shmID, IPC_RMID, NULL);
}

void threadHandler::initializeSemaphores() {
    // Mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init failed");
        exit(EXIT_FAILURE);
    }
    // Full - init 0
    if (sem_init(&semFull, 0, 0))
    	{
       	perror("Sempahore create error");
       	exit(EXIT_FAILURE);
    } 
    // Empty - init nitems
    if (sem_init(&semEmpty, 0, nitems))
    	{
       	perror("Sempahore create error");
       	exit(EXIT_FAILURE);
    } 
}

void threadHandler::deleteSemaphores() {
    // destroy mutex + semaphores
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semFull);
    sem_destroy(&semEmpty);
}

void threadHandler::exitOnFailure() {
    deleteSharedMem();
    deleteSemaphores();
    cout << "System cleaned, forcing termination..." << endl;
    exit(EXIT_FAILURE);
}

void* threadHandler::producerThread(void *arg) {
    short int seqenceNum = 0;    
    short int chkSum;
    int offset;

    for (int k = 0; k < ntimes; k ++) {     // Go through process ntimes
        for (int j = 0; j < nitems; j++) {
            offset = j * 32;

            // Critical Section --- Lock while writing ---------------------------------------
            sem_wait(&semEmpty);
            pthread_mutex_lock(&mutex);

            // Add sequence number to the buffer
            memcpy(&shmBuf[offset], &seqenceNum, 2);

            // Write random data to buffer
            for (int i = 2 + offset; i < (BUFSIZE-4) + offset; i++) {
                shmBuf[i] = (unsigned char)(rand() % 256);
            }

            // Calc and Insert Checksum
            chkSum = checksum(&shmBuf[2 + offset], 28);
            memcpy(&shmBuf[30 + offset], &chkSum, 2);

            // Critical Section finished ---- Unlock -----------------------------------------
            pthread_mutex_unlock(&mutex);
            sem_post(&semFull);

            seqenceNum++;
        }
    }
    
    // Producers job is done, unlock mutex, terminate thread
    //cout << "EXIT PRODUCER" << endl;
    pthread_exit(NULL);
}

void* threadHandler::consumerThread(void *arg) {
    short int sequenceNum = 0;
    short int chkSum;
    int offset;
    short int recievedSequenceNum;
    short int recievedCSum;

    for (int k = 0; k < ntimes; k++) {
        for (int j = 0; j < nitems; j++) {
            offset = 32 * j;

            // Critical Section --- Lock while reading ----------
            sem_wait(&semFull);
            pthread_mutex_lock(&mutex);

            // Get Sequence num
            memcpy(&recievedSequenceNum, &shmBuf[0 + offset], 2);

            // Calculate checksum
            chkSum = checksum(&shmBuf[2 + offset], 28);

            // Get checksum
            memcpy(&recievedCSum, &shmBuf[30 + offset], 2);

            // Compare checksum and sequence num TODO -- add a system terminate
            if (recievedCSum != chkSum) {
                cout << "CHECKSUM MISMATCH" << endl;
                cout << "Recieved cSum: " << recievedCSum << endl;
                cout << "Recieved cSum: " << chkSum << endl;
                exitOnFailure();
            }
            if (recievedSequenceNum != sequenceNum) {
                cout << "SEQENCENUM MISMATCH" << endl;
                cout << "Recieved sNum: " << recievedSequenceNum << endl;
                cout << "Recieved sNum: " << sequenceNum << endl;
                exitOnFailure();
            }

            // Critical Section done -------------------------------
            pthread_mutex_unlock(&mutex);
            sem_post(&semEmpty);

            sequenceNum++;
        }    
    }

    // Consumer is done, terminate thread
    //cout << "EXIT CONSUMER" << endl;
    pthread_exit(NULL);
}

#endif // THREADHANDLER_H