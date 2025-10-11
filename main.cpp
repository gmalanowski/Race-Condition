#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <mutex>
#include "ArrayFunctions.h"
#include "SortingAlgorithms.h"
#include "DataGenerator.h"
#include "CsvFileHandler.h"

using namespace std;

int main() {
    /*=============================================================*/
    /* ---------------- CONFIGURATION -----------------------------*/
    const int num_threads = 300;
    ThreadPool pool(num_threads);

    int arraySize = 200000;
    int minValue = -100000;
    int maxValue = 300000;
    float percentageSorted = 0.0f;
    string direction = "asc"; // "asc" or "desc"
    string fileName = "Results.csv";

    cout << "=== THREADING EXPERIMENT ===" << endl;
    cout << "Threads: " << num_threads << ", Array size: " << arraySize << endl;

    CsvFileHandler::beginCSV(fileName, arraySize);

    /*=============================================================*/
    /* ---------------- SINGLE-THREAD TEST ------------------------*/
    vector<int> array = DataGenerator::generateInt(arraySize, minValue, maxValue, percentageSorted, direction);

    auto startSingle = chrono::high_resolution_clock::now();
    SortingAlgorithms::mergeSort(array, 0, array.size() - 1);
    auto endSingle = chrono::high_resolution_clock::now();
    double timeSingle = chrono::duration<double>(endSingle - startSingle).count() * 1000.0;
    bool singleSorted = ArrayFunctions::sortedAscending(array);

    cout << "\n[1] Single-threaded merge sort" << endl;
    cout << "Time: " << timeSingle << " ms" << (singleSorted ? " ✅" : " ❌") << endl;

    /*=============================================================*/
    /* ---------------- MULTI-THREAD TEST -------------------------*/
    vector<int> arrayParallel = DataGenerator::generateInt(arraySize, minValue, maxValue, percentageSorted, direction);

    auto startParallel = chrono::high_resolution_clock::now();
    SortingAlgorithms::parallelMergeSort(pool, arrayParallel, 0, arrayParallel.size() - 1);
    auto endParallel = chrono::high_resolution_clock::now();
    double timeParallel = chrono::duration<double>(endParallel - startParallel).count() * 1000.0;
    bool parallelSorted = ArrayFunctions::sortedAscending(arrayParallel);

    cout << "\n[2] Multi-threaded merge sort (safe)" << endl;
    cout << "Time: " << timeParallel << " ms" << (parallelSorted ? " ✅" : " ❌") << endl;

    /*=============================================================*/
    /* ---------------- RACE CONDITION DEMO -----------------------*/
    cout << "\n[3] Race condition vs synchronized access" << endl;

    ThreadPool racePool(num_threads);
    int counter_race = 0;
    int counter_safe = 0;
    std::mutex counter_mutex;
    const int n_tasks = 1000;

    vector<future<void>> futures_race;
    vector<future<void>> futures_safe;

    // (a) Race condition — no synchronization
    for (int i = 0; i < n_tasks; ++i) {
        futures_race.push_back(racePool.submit([&counter_race]() {
            counter_race++; // UNSAFE access to shared data
        }));
    }

    // (b) Safe version — with mutex
    for (int i = 0; i < n_tasks; ++i) {
        futures_safe.push_back(racePool.submit([&counter_safe, &counter_mutex]() {
            std::lock_guard<std::mutex> lock(counter_mutex);
            counter_safe++;
        }));
    }

    // Wait for both groups
    for (auto& f : futures_race) f.get();
    for (auto& f : futures_safe) f.get();

    cout << "Expected value: " << n_tasks << endl;
    cout << "Race-condition counter: " << counter_race << " ❌" << endl;
    cout << "Synchronized counter:   " << counter_safe << " ✅" << endl;

    /*=============================================================*/
    /* ---------------- SAVE RESULTS TO CSV -----------------------*/
    CsvFileHandler::saveTo(fileName,
        timeSingle, singleSorted,
        timeParallel, parallelSorted,
        num_threads, counter_race, counter_safe, arraySize);

    cout << "\nResults saved to " << fileName << endl;

    /*=============================================================*/
    /* ---------------- SUMMARY ----------------------------------*/
    cout << "\n=== SUMMARY ===" << endl;
    cout << "Single-thread sort time:   " << timeSingle   << " ms" << endl;
    cout << "Multi-thread sort time:    " << timeParallel << " ms" << endl;
    cout << "Race condition demo done — see above results." << endl;

    cout << "\nProgram finished successfully." << endl;
    return 0;
}
