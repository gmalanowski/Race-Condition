#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <string> // Added for std::string
#include <vector>
#include "ArrayFuntions.h"
#include "SortingAlgorithms.h"
#include "DataGenerator.h"
#include "CsvFileHandler.h"

using namespace std;
int main() {
    /*=============================================================*/
    /* ----------------- CONFIG -----------------------------------*/

    const int num_threads = 300;
    ThreadPool pool(num_threads);

    int n_runs = 1000;
    int arraySize = 1000000;
    int arrayType = 1; /* 1 = int, 2 = float */
    int minValue = -100000;
    int maxValue = 300000;
    float percentageSorted = 0;
    string direction = "asc"; /* "asc" = ascending, "desc" = descending" */
    string fileName = "Wyniki.csv";

    /*=============================================================*/
    CsvFileHandler::beginCSV(fileName, arraySize);
    for (int i = 0; i < n_runs; i++) {
        if (arrayType == 1) {
            vector<int> array = DataGenerator::generateInt(arraySize, minValue, maxValue, percentageSorted, direction);
            vector<int> array_pararell = array;

            auto startMergeSort = chrono::high_resolution_clock::now();
            SortingAlgorithms::mergeSort(array, 0, array.size() - 1);
            auto endMergeSort = chrono::high_resolution_clock::now();
            double time = chrono::duration<double>(endMergeSort - startMergeSort).count() * 1000.0;
            cout << "Serial: " << time << " ms" << endl;
            bool serialSorted = ArrayFuntions::sortedAscending(array);

            auto startMergeSortPararell = chrono::high_resolution_clock::now();
            SortingAlgorithms::parallelMergeSort(pool, array_pararell, 0, array_pararell.size() - 1);
            auto endMergeSortPararell = chrono::high_resolution_clock::now();
            double timeParallel = chrono::duration<double>(endMergeSortPararell - startMergeSortPararell).count() * 1000.0;
            cout << "Parallel: "<< timeParallel << " ms" << endl;
            bool parallelSorted = ArrayFuntions::sortedAscending(array);

            CsvFileHandler::saveTo(fileName, time, serialSorted, timeParallel, parallelSorted, i);
        }
        if (arrayType == 2) {
            vector<float> array = DataGenerator::generateFloat(arraySize, minValue, maxValue, percentageSorted, direction);
            vector<float> array_pararell = array;

            auto startMergeSort = chrono::high_resolution_clock::now();
            SortingAlgorithms::mergeSort(array, 0, array.size() - 1);
            auto endMergeSort = chrono::high_resolution_clock::now();
            double time = chrono::duration<double>(endMergeSort - startMergeSort).count() * 1000.0;
            cout << "Serial: " << time << " ms" << endl;
            bool serialSorted = ArrayFuntions::sortedAscending(array);

            auto startMergeSortPararell = chrono::high_resolution_clock::now();
            SortingAlgorithms::parallelMergeSort(pool, array_pararell, 0, array_pararell.size() - 1);
            auto endMergeSortPararell = chrono::high_resolution_clock::now();
            double timeParallel = chrono::duration<double>(endMergeSortPararell - startMergeSortPararell).count() * 1000.0;
            cout << "Parallel: "<< timeParallel << " ms" << endl;
            bool parallelSorted = ArrayFuntions::sortedAscending(array);

            CsvFileHandler::saveTo(fileName, time, serialSorted, timeParallel, parallelSorted, i);
        }
    }
    return 0;
}
