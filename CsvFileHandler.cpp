#include "CsvFileHandler.h"
#include <fstream>
#include <iostream>

using namespace std;

void CsvFileHandler::beginCSV(const string& csvName, int size) {
    ofstream file(csvName, ios::out); // overwrite old file
    if (!file.is_open()) {
        cerr << "Error: could not open CSV file for writing." << endl;
        return;
    }

    file << "Run,"
         << "Serial Time (ms),Serial Sorted,"
         << "Parallel Time (ms),Parallel Sorted,"
         << "Threads,"
         << "Race Counter,"
         << "Safe Counter,"
         << "Array Size" << endl;

    file.close();
}

void CsvFileHandler::saveTo(const string& csvName,
                            double serialTime, bool serialSorted,
                            double parallelTime, bool parallelSorted,
                            int numThreads, int raceValue, int safeValue, int arraySize) {
    static int runNumber = 0;
    ofstream file(csvName, ios::app);
    if (!file.is_open()) {
        cerr << "Error: could not open CSV file for appending." << endl;
        return;
    }

    file << runNumber++ << ","
         << serialTime << ","
         << (serialSorted ? "true" : "false") << ","
         << parallelTime << ","
         << (parallelSorted ? "true" : "false") << ","
         << numThreads << ","
         << raceValue << ","
         << safeValue << ","
         << arraySize << endl;

    file.close();
}
