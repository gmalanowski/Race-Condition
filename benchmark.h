#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <string>
#include <vector>
#include <functional>
#include "config.h"

// Structure to hold benchmark results for a single run
struct BenchmarkResult {
    std::string algorithm_name;
    size_t array_size;
    int iteration;
    double time_microseconds;
    bool is_sorted;
};

// Structure to hold statistical summary for an algorithm
struct AlgorithmStats {
    std::string algorithm_name;
    double avg_time_microseconds;
    double min_time_microseconds;
    double max_time_microseconds;
    double std_dev_microseconds;
    int successful_sorts;
    int total_runs;
};

// Benchmark runner class
class BenchmarkRunner {
public:
    explicit BenchmarkRunner(const BenchmarkConfig &config);

    // Run a single algorithm benchmark
    void runAlgorithm(
        const std::string &algorithm_name,
        std::function<void(std::vector<int> &)> sort_function
    );

    // Export all results to CSV
    void exportToCSV() const;

    // Print summary statistics
    void printSummary() const;

private:
    BenchmarkConfig config_;
    std::vector<BenchmarkResult> results_;

    std::vector<int> generateRandomArray() const;

    AlgorithmStats calculateStats(const std::string &algorithm_name) const;
};

#endif // BENCHMARK_H