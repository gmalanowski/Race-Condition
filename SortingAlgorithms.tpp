#ifndef SORTINGALGORITHMS_TPP
#define SORTINGALGORITHMS_TPP

#include <cmath>
#include "SortingAlgorithms.h"
using namespace std;

template <NumericType T>
void SortingAlgorithms::merge(vector<T>& arr, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;

    vector<T> temp_left(left_size);
    vector<T> temp_right(right_size);

    for (int i = 0; i < left_size; i++) {
        temp_left[i] = arr[left + i];
    }
    for (int i = 0; i < right_size; i++) {
        temp_right[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0;
    int k = left;
    while (i < left_size && j < right_size) {
        if (temp_left[i] <= temp_right[j]) {
            arr[k] = temp_left[i];
            i++;
        }
        else {
            arr[k] = temp_right[j];
            j++;
        }
        k++;
    }
    while (i < left_size) {
        arr[k] = temp_left[i];
        i++;
        k++;
    }
    while (j < right_size) {
        arr[k] = temp_right[j];
        j++;
        k++;
    }
}

template <NumericType T>
void SortingAlgorithms::mergeSort(vector<T>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

template <NumericType T>
void SortingAlgorithms::parallelMergeSort(ThreadPool& pool, vector<T>& arr, int left, int right) {
    // If a subarray is small, sorting it sequentially is faster and prevents thread pool deadlock.
    const int SEQUENTIAL_THRESHOLD = 2048;

    if (left >= right) {
        return;
    }

    // If the chunk is below the threshold, switch to the sequential version.
    if ((right - left + 1) <= SEQUENTIAL_THRESHOLD) {
        mergeSort(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;

    // We must explicitly cast the function pointer to tell the compiler exactly
    // which function template instantiation we want to use.
    auto future = pool.submit(static_cast<void(*)(ThreadPool&, vector<T>&, int, int)>(parallelMergeSort), std::ref(pool), std::ref(arr), left, mid);

    // While the left half is sorting, the current thread can sort the right half.
    parallelMergeSort(pool, arr, mid + 1, right);

    // Wait for the left half to be finished before merging.
    future.get();

    merge(arr, left, mid, right);
}




#endif // SORTINGALGORITHMS_TPP
