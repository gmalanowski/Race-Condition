#ifndef SORTINGALGORITHMS_H
#define SORTINGALGORITHMS_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include "ThreadPool.h"

template <typename T>
concept NumericType = std::integral<T> || std::floating_point<T>; // limits the use to int and float types
class SortingAlgorithms {
public:

    template <NumericType T>
    static void merge(std::vector<T>& arr, int left, int mid, int right);

    template <NumericType T>
    static void mergeSort(std::vector<T>& arr, int left, int right);

    template <NumericType T>
    static void parallelMergeSort(ThreadPool& pool, std::vector<T>& arr, int left, int right);

};


#include "SortingAlgorithms.tpp"

#endif // SORTINGALGORITHMS_H
