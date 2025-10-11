#ifndef SORTINGALGORITHMS_TPP
#define SORTINGALGORITHMS_TPP

#include "SortingAlgorithms.h"
#include <vector>   // make explicit, since this is a template TU

// Merge two sorted subarrays: [left..mid] and [mid+1..right] into arr[left..right].
// Stable: when equal, elements from the left half are taken first.
template <NumericType T>
void SortingAlgorithms::merge(std::vector<T>& arr, int left, int mid, int right) {
    const int left_size  = mid - left + 1;
    const int right_size = right - mid;

    std::vector<T> temp_left(static_cast<std::size_t>(left_size));
    std::vector<T> temp_right(static_cast<std::size_t>(right_size));

    // Copy data into temporary buffers
    for (int i = 0; i < left_size; ++i) {
        temp_left[static_cast<std::size_t>(i)] = arr[static_cast<std::size_t>(left + i)];
    }
    for (int i = 0; i < right_size; ++i) {
        temp_right[static_cast<std::size_t>(i)] = arr[static_cast<std::size_t>(mid + 1 + i)];
    }

    // Merge back into arr[left..right]
    int i = 0, j = 0;
    int k = left;

    while (i < left_size && j < right_size) {
        if (temp_left[static_cast<std::size_t>(i)] <= temp_right[static_cast<std::size_t>(j)]) {
            arr[static_cast<std::size_t>(k++)] = temp_left[static_cast<std::size_t>(i++)];
        } else {
            arr[static_cast<std::size_t>(k++)] = temp_right[static_cast<std::size_t>(j++)];
        }
    }
    // Copy remaining elements (at most one of the loops below executes)
    while (i < left_size) {
        arr[static_cast<std::size_t>(k++)] = temp_left[static_cast<std::size_t>(i++)];
    }
    while (j < right_size) {
        arr[static_cast<std::size_t>(k++)] = temp_right[static_cast<std::size_t>(j++)];
    }
}

// Classic single-threaded merge sort on arr[left..right]
template <NumericType T>
void SortingAlgorithms::mergeSort(std::vector<T>& arr, int left, int right) {
    if (left >= right) return;
    const int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Parallel merge sort using a ThreadPool.
// Strategy: spawn LEFT half into the pool, sort RIGHT half in the current thread.
// This keeps parallelism while avoiding spawning too many tasks.
template <NumericType T>
void SortingAlgorithms::parallelMergeSort(ThreadPool& pool, std::vector<T>& arr, int left, int right) {
    // For small chunks, sequential is faster and prevents excessive task overhead.
    constexpr int SEQUENTIAL_THRESHOLD = 2048;

    if (left >= right) return;

    const int len = right - left + 1;
    if (len <= SEQUENTIAL_THRESHOLD) {
        mergeSort(arr, left, right);
        return;
    }

    const int mid = left + (right - left) / 2;

    // Spawn the left half into the pool using a lambda (clearer than function pointer casting).
    auto futLeft = pool.submit([&pool, &arr, left, mid]() {
        SortingAlgorithms::parallelMergeSort<T>(pool, arr, left, mid);
    });

    // Sort the right half in the current thread.
    SortingAlgorithms::parallelMergeSort<T>(pool, arr, mid + 1, right);

    // Wait for the left task to finish before merging.
    futLeft.get();

    // Now merge the two sorted halves.
    merge(arr, left, mid, right);
}

#endif // SORTINGALGORITHMS_TPP
