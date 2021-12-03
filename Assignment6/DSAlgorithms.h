#ifndef DSALGORITHMS_H
#define DSALGORITHMS_H

#include <iostream>
#include <list>
#include <iterator>
using namespace std;

class DSAlgorithms {
public:
    int FCFS(int initPos, int arr[], int arrSize);   // returns amount of head movement
    int SSTF(int initPos, int arr[], int arrSize);   // returns amount of head movement
    int SCAN(int initPos, int arr[], int arrSize);   // returns amount of head movement
    int CSCAN(int initPos, int arr[], int arrSize);  // returns amount of head movement
};

int DSAlgorithms::FCFS(int initPos, int arr[], int arrSize) {
    int counter = 0;
    for (int i = 0; i < arrSize; i++) {
        counter += abs(initPos - arr[i]);
        initPos = arr[i];
    }
    return counter;
}

int DSAlgorithms::SSTF(int initPos, int arr[], int arrSize) {
    int counter = 0;
    int upper, lower;
    list<int> sList;
    for (int i = 0; i < 1000; i++) {
        sList.push_back(arr[i]);
    }
    sList.sort();
    list<int>::iterator iter;
    
    while (!sList.empty()) {
        iter = sList.begin();
        while (*iter < initPos && iter != sList.end()) {
            iter++;
        }

        if (iter == sList.end()) {
            iter--;
        }
        upper = *iter;

        if (iter != sList.begin()) {
            iter--; 
        }       
        lower = *iter;

        if ((upper - initPos) <= (initPos - lower)) {
            counter += abs(upper - initPos);
            initPos = upper;
            sList.remove(upper);
        }
        else {
            counter += abs(initPos - lower);
            initPos = lower;
            sList.remove(lower);
        }
    }
    return counter;
}

int DSAlgorithms::SCAN(int initPos, int arr[], int arrSize) {
    bool goingDown = false;
    int val = initPos;
    int oldVal;
    int counter = 0;

    // Initalize list
    list<int> sList;
    for (int i = 0; i < 1000; i++) {
        sList.push_back(arr[i]);
    }
    sList.sort();
    list<int>::iterator iter;

    // Choose starting direction
    if (initPos < 2500 && initPos != *sList.begin()) {
        goingDown = true;
    }

    // Seek to location
    iter = sList.begin();
    while (*iter < initPos && iter != sList.end()) {
        iter++;
    }
    if (iter == sList.end() || (*iter > initPos && goingDown)) {
        iter--;
    }

    // Perform SCAN + calc head movement
    while (!sList.empty()) {
        if (goingDown) {
            oldVal = val;
            val = *iter;
            while (*iter >= val && iter != sList.begin()) {
                iter--;
            }
            sList.remove(val);
            if (iter == sList.begin()) {
                goingDown = false;
            }
            counter += abs(val - oldVal);
        }
        else {
            oldVal = val;
            val = *iter;
            while (*iter == val) {
               iter++; 
            }            
            sList.remove(val);
            if (iter == sList.end()) {
                goingDown = true;
                iter--;
            }
            counter += abs(val - oldVal);
        }
    }
    return counter;
}

int DSAlgorithms::CSCAN(int initPos, int arr[], int arrSize) {
    int counter = 0;
    int val = initPos;
    int oldVal;

    // Init List
    list<int> sList;
    for (int i = 0; i < 1000; i++) {
        sList.push_back(arr[i]);
    }
    sList.sort();
    list<int>::iterator iter;

    // Seek to starting postion
    iter = sList.begin();
    while (*iter < initPos && iter != sList.end()) {
        iter++;
    }
    if (iter == sList.end()) {
        iter--;
        if (*iter < initPos) {
            iter = sList.begin();
        }
    }

    // Perform CSCAN
    while (!sList.empty()) {
        oldVal = val;
        val = *iter;
        while (*iter == val) {
            iter++; 
        }            
        sList.remove(val);
        if (iter == sList.end()) {
            iter = sList.begin();
        }
        counter += abs(val - oldVal);
    }
    return counter;
}



#endif //DSALGORITHMS_H