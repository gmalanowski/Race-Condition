#include "benchmark.h"
#include "sorting_algorithms.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <cmath>
#include <algorithm>

BenchmarkRunner::BenchmarkRunner(const BenchmarkConfig &config)
    : config_(config) {
}

std::vector<int> BenchmarkRunner::generateRandomArray() const {
    std::vector<int> arr(config_.array_size);
    std::mt19937 gen(config_.random_seed);
    std::uniform_int_distribution<> dis(1, 1000000);

    for (size_t i = 0; i < config_.array_size; i++) {
        arr[i] = dis(gen);
    }

    return arr;
}

void BenchmarkRunner::runAlgorithm(
    const std::string &algorithm_name,
    std::function<void(std::vector<int> &)> sort_function
) {
    std::cout << "Running " << algorithm_name << "..." << std::endl;

    for (int i = 0; i < config_.iterations; i++) {
        // Generate a fresh random array for each iteration
        std::vector<int> arr = generateRandomArray();

        // Measure execution time
        auto start = std::chrono::high_resolution_clock::now();
        sort_function(arr);
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate elapsed time in microseconds
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_us = duration.count();

        // Verify correctness
        bool is_sorted = isSorted(arr);

        // Store result
        BenchmarkResult result;
        result.algorithm_name = algorithm_name;
        result.array_size = config_.array_size;
        result.iteration = i + 1;
        result.time_microseconds = time_us;
        result.is_sorted = is_sorted;

        results_.push_back(result);

        std::cout << "  Iteration " << (i + 1) << "/" << config_.iterations
                << ": " << std::fixed << std::setprecision(2)
                << time_us / 1000.0 << " ms"
                << (is_sorted ? " [PASS]" : " [FAIL]")
                << std::endl;

        if (!is_sorted) {
            std::cerr << "  WARNING: Array is not properly sorted!" << std::endl;
        }
    }

    std::cout << std::endl;
}

AlgorithmStats BenchmarkRunner::calculateStats(const std::string &algorithm_name) const {
    AlgorithmStats stats;
    stats.algorithm_name = algorithm_name;
    stats.total_runs = 0;
    stats.successful_sorts = 0;

    std::vector<double> times;

    // Collect all times for this algorithm
    for (const auto &result: results_) {
        if (result.algorithm_name == algorithm_name) {
            times.push_back(result.time_microseconds);
            stats.total_runs++;
            if (result.is_sorted) {
                stats.successful_sorts++;
            }
        }
    }

    if (times.empty()) {
        stats.avg_time_microseconds = 0.0;
        stats.min_time_microseconds = 0.0;
        stats.max_time_microseconds = 0.0;
        stats.std_dev_microseconds = 0.0;
        return stats;
    }

    // Calculate statistics
    stats.min_time_microseconds = *std::min_element(times.begin(), times.end());
    stats.max_time_microseconds = *std::max_element(times.begin(), times.end());

    double sum = 0.0;
    for (double time: times) {
        sum += time;
    }
    stats.avg_time_microseconds = sum / times.size();

    // Calculate standard deviation
    double variance_sum = 0.0;
    for (double time: times) {
        double diff = time - stats.avg_time_microseconds;
        variance_sum += diff * diff;
    }
    stats.std_dev_microseconds = std::sqrt(variance_sum / times.size());

    return stats;
}

void BenchmarkRunner::exportToCSV() const {
    std::ofstream file(config_.output_file);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open output file " << config_.output_file << std::endl;
        return;
    }

    // Write CSV header
    file << "Algorithm,ArraySize,Iteration,TimeMicroseconds,TimeMilliseconds,IsSorted\n";

    // Write data rows
    for (const auto &result: results_) {
        file << result.algorithm_name << ","
                << result.array_size << ","
                << result.iteration << ","
                << std::fixed << std::setprecision(2) << result.time_microseconds << ","
                << std::fixed << std::setprecision(2) << result.time_microseconds / 1000.0 << ","
                << (result.is_sorted ? "true" : "false") << "\n";
    }

    file.close();
    std::cout << "Results exported to " << config_.output_file << std::endl;
}

void BenchmarkRunner::printSummary() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "BENCHMARK SUMMARY" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Array size: " << config_.array_size << " elements" << std::endl;
    std::cout << "Iterations per algorithm: " << config_.iterations << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Get unique algorithm names
    std::vector<std::string> algorithm_names;
    for (const auto &result: results_) {
        if (std::find(algorithm_names.begin(), algorithm_names.end(), result.algorithm_name) == algorithm_names.end()) {
            algorithm_names.push_back(result.algorithm_name);
        }
    }

    // Calculate and display statistics for each algorithm
    std::vector<AlgorithmStats> all_stats;
    for (const auto &name: algorithm_names) {
        AlgorithmStats stats = calculateStats(name);
        all_stats.push_back(stats);

        std::cout << name << ":" << std::endl;
        std::cout << "  Average: " << std::fixed << std::setprecision(2)
                << stats.avg_time_microseconds / 1000.0 << " ms" << std::endl;
        std::cout << "  Min:     " << std::fixed << std::setprecision(2)
                << stats.min_time_microseconds / 1000.0 << " ms" << std::endl;
        std::cout << "  Max:     " << std::fixed << std::setprecision(2)
                << stats.max_time_microseconds / 1000.0 << " ms" << std::endl;
        std::cout << "  StdDev:  " << std::fixed << std::setprecision(2)
                << stats.std_dev_microseconds / 1000.0 << " ms" << std::endl;
        std::cout << "  Success: " << stats.successful_sorts << "/" << stats.total_runs << std::endl;
        std::cout << std::endl;
    }

    // Calculate speedup comparisons
    if (all_stats.size() >= 2) {
        std::cout << "========================================" << std::endl;
        std::cout << "SPEEDUP ANALYSIS" << std::endl;
        std::cout << "========================================" << std::endl;

        // Find single-threaded and multi-threaded merge sort
        AlgorithmStats *single_threaded = nullptr;
        AlgorithmStats *multi_threaded = nullptr;

        for (auto &stats: all_stats) {
            if (stats.algorithm_name.find("Single-Threaded Merge Sort") != std::string::npos) {
                single_threaded = &stats;
            } else if (stats.algorithm_name.find("Multi-Threaded Merge Sort") != std::string::npos) {
                multi_threaded = &stats;
            }
        }

        if (single_threaded && multi_threaded) {
            double speedup = single_threaded->avg_time_microseconds / multi_threaded->avg_time_microseconds;
            std::cout << "Multi-threaded vs Single-threaded Merge Sort:" << std::endl;
            std::cout << "  Speedup: " << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
            std::cout << std::endl;
        }

        // Compare all algorithms to fastest
        double fastest_time = all_stats[0].avg_time_microseconds;
        std::string fastest_name = all_stats[0].algorithm_name;

        for (const auto &stats: all_stats) {
            if (stats.avg_time_microseconds < fastest_time) {
                fastest_time = stats.avg_time_microseconds;
                fastest_name = stats.algorithm_name;
            }
        }

        std::cout << "Fastest algorithm: " << fastest_name << std::endl;
        std::cout << "Relative performance:" << std::endl;
        for (const auto &stats: all_stats) {
            double ratio = stats.avg_time_microseconds / fastest_time;
            std::cout << "  " << stats.algorithm_name << ": "
                    << std::fixed << std::setprecision(2) << ratio << "x slower" << std::endl;
        }
        std::cout << std::endl;
    }
}