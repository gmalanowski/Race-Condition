#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>

// Single-threaded merge sort
void mergeSortSingleThreaded(std::vector<int> & arr);

// Multi-threaded merge sort
void mergeSortMultiThreaded(std::vector<int> &arr, int thread_count);

// Quick sort (single-threaded)
void quickSort(std::vector<int> & arr);

// Heap sort (single-threaded)
void heapSort(std::vector<int> & arr);

// STL sort (for reference/baseline)
void stlSort(std::vector<int> & arr);

// Utility function to verify if array is sorted
bool isSorted(const std::vector<int> &arr);

#endif // SORTING_ALGORITHMS_H