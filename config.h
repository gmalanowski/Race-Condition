#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>

struct BenchmarkConfig {
    size_t array_size = 1000000;
    int iterations = 25;
    int thread_count = 8;
    unsigned int random_seed = 42;
    const char *output_file = "benchmark_results.csv";
};

#endif // CONFIG_H
