#include "sorting_algorithms.h"
#include "ThreadPool.h"
#include <algorithm>
#include <thread>
#include <vector>
#include <future>

// ============================================
// Helper functions for merge sort
// ============================================

void merge(std::vector<int> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortHelper(std::vector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// ============================================
// Single-threaded merge sort
// ============================================

void mergeSortSingleThreaded(std::vector<int> &arr) {
    if (arr.size() > 1) {
        mergeSortHelper(arr, 0, arr.size() - 1);
    }
}

// ============================================
// Multi-threaded merge sort
// ============================================

void mergeSortMultiThreadedHelper(std::vector<int> &arr, int left, int right, int depth, int max_depth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Use threads only up to max_depth to avoid thread explosion
        if (depth < max_depth) {
            std::thread leftThread(mergeSortMultiThreadedHelper, std::ref(arr), left, mid, depth + 1, max_depth);
            std::thread rightThread(mergeSortMultiThreadedHelper, std::ref(arr), mid + 1, right, depth + 1, max_depth);

            leftThread.join();
            rightThread.join();
        } else {
            // Fall back to single-threaded for smaller subarrays
            mergeSortHelper(arr, left, mid);
            mergeSortHelper(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

void mergeSortMultiThreaded(std::vector<int> &arr, int thread_count) {
    if (arr.size() > 1) {
        // Calculate max depth based on desired thread count
        // Each level doubles the number of threads: 2^depth = thread_count
        int max_depth = 0;
        int threads = 1;
        while (threads < thread_count) {
            threads *= 2;
            max_depth++;
        }

        mergeSortMultiThreadedHelper(arr, 0, arr.size() - 1, 0, max_depth);
    }
}

// ============================================
// Quick sort
// ============================================

int partition(std::vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int> &arr) {
    if (arr.size() > 1) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

// ============================================
// Heap sort
// ============================================

void heapify(std::vector<int> &arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int> &arr) {
    int n = arr.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ============================================
// STL sort
// ============================================

void stlSort(std::vector<int> &arr) {
    std::sort(arr.begin(), arr.end());
}

// ============================================
// ThreadPool-based merge sort
// ============================================

void mergeSortThreadPool(std::vector<int> &arr, ThreadPool &pool) {
    if (arr.size() <= 1) return;

    // Determine the number of chunks to split into
    // Use a reasonable chunk size to avoid too much overhead
    constexpr size_t MIN_CHUNK_SIZE = 2048;
    size_t num_chunks = (arr.size() + MIN_CHUNK_SIZE - 1) / MIN_CHUNK_SIZE;

    // If only one chunk, just sort sequentially
    if (num_chunks <= 1) {
        mergeSortHelper(arr, 0, arr.size() - 1);
        return;
    }

    // Calculate actual chunk size
    size_t chunk_size = (arr.size() + num_chunks - 1) / num_chunks;

    // Sort each chunk in parallel using the thread pool
    std::vector<std::future<void>> futures;
    futures.reserve(num_chunks);

    for (size_t i = 0; i < num_chunks; ++i) {
        size_t start = i * chunk_size;
        size_t end = std::min(start + chunk_size - 1, arr.size() - 1);

        if (start <= end) {
            futures.push_back(pool.submit([&arr, start, end]() {
                mergeSortHelper(arr, start, end);
            }));
        }
    }

    // Wait for all chunks to be sorted
    for (auto &fut : futures) {
        fut.get();
    }

    // Now merge the sorted chunks iteratively
    // This is done sequentially on the main thread (no blocking in worker threads!)
    size_t current_chunk_size = chunk_size;
    while (current_chunk_size < arr.size()) {
        for (size_t i = 0; i < arr.size(); i += 2 * current_chunk_size) {
            size_t left = i;
            size_t mid = std::min(i + current_chunk_size - 1, arr.size() - 1);
            size_t right = std::min(i + 2 * current_chunk_size - 1, arr.size() - 1);

            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
        current_chunk_size *= 2;
    }
}

// ============================================
// Verification function
// ============================================

bool isSorted(const std::vector<int> &arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}