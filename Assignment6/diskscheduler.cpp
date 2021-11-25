#include <random>
#include "DSAlgorithms.h"
#include "FileHandler.h"
#include <iostream>
using namespace std;

void createRandomRequests(int arr[], int elmCount) {
    for (int i = 0; i < elmCount; i++) {
        arr[i] = rand() % 5000;
    }
}

int main(int argc, char* argv[]) {
    // argument handling
    if (argc != 3) {
        cout << "Incorrect num of arguments..." << endl;
        cout << "Use format ./diskscheduler initPosition filePath" << endl;
        return 0;
    }
    else if (stoi(argv[1]) > 5000 || stoi(argv[1]) < 0) {
        cout << "Incorrect head positon..." << endl;
        cout << "Must be between 0-5000" << endl;
        return 0;
    }

    // Process arguments + copy from file
    int serviceRequests[1000];
    int arrSize = 1000;
    FileHandler(argv[2], serviceRequests);

    // Test algorithms and display results
    DSAlgorithms tester;
    cout << "FCFS results in " << tester.FCFS(stoi(argv[1]), serviceRequests, arrSize) << " head movements." << endl;
    cout << "SSTF results in " << tester.SSTF(stoi(argv[1]), serviceRequests, arrSize) << " head movements." << endl;
    cout << "SCAN results in " << tester.SCAN(stoi(argv[1]), serviceRequests, arrSize) << " head movements." << endl;
    cout << "CSCAN results in " << tester.CSCAN(stoi(argv[1]), serviceRequests, arrSize) << " head movements." << endl;

    return 0;
}