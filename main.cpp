#include <iostream>
#include <functional>
#include "config.h"
#include "benchmark.h"
#include "sorting_algorithms.h"

int main() {
    BenchmarkConfig config;

    std::cout << "========================================" << std::endl;
    std::cout << "SORTING ALGORITHM BENCHMARK" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Array size: " << config.array_size << " elements" << std::endl;
    std::cout << "  Iterations: " << config.iterations << " per algorithm" << std::endl;
    std::cout << "  Thread count: " << config.thread_count << std::endl;
    std::cout << "  Output file: " << config.output_file << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Create benchmark runner
    BenchmarkRunner benchmark(config);

    // Run benchmarks for each algorithm
    benchmark.runAlgorithm("Single-Threaded Merge Sort", mergeSortSingleThreaded);

    benchmark.runAlgorithm("Multi-Threaded Merge Sort", [&config](std::vector<int> &arr) {
        mergeSortMultiThreaded(arr, config.thread_count);
    });

    benchmark.runAlgorithm("Quick Sort", quickSort);

    benchmark.runAlgorithm("Heap Sort", heapSort);

    benchmark.runAlgorithm("STL Sort (std::sort)", stlSort);

    // Print summary statistics
    benchmark.printSummary();

    // Export results to CSV
    benchmark.exportToCSV();

    std::cout << "\nBenchmark completed successfully!" << std::endl;

    return 0;
}
