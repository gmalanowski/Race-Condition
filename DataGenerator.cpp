#include "DataGenerator.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>  // std::min, std::max, std::swap
#include <cmath>      // std::lround
#include <string>

namespace {
    // Initialize the random seed once per program run
    inline void seed_once() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
    }

    // Clamp integer to the range [lo, hi]
    inline int clamp_int(int v, int lo, int hi) {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    // Convert percentage (0.0–1.0) to number of sorted elements
    inline int sorted_count(int n, float pct) {
        if (n <= 0) return 0;
        if (pct < 0.f) pct = 0.f;
        if (pct > 1.f) pct = 1.f;
        int k = static_cast<int>(std::lround(n * pct));
        if (k < 0) k = 0;
        if (k > n) k = n;
        return k;
    }

    // Convert a string to lowercase (for easy comparison)
    inline std::string to_lower(std::string s) {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }
}

std::vector<int> DataGenerator::generateInt(
    int dataArraySize,
    int dataRangeMin,
    int dataRangeMax,
    float percentageSorted,
    std::string sortingDirection)
{
    seed_once();  // Make sure rand() is initialized only once

    // Fix invalid arguments
    if (dataArraySize < 0) dataArraySize = 0;
    if (dataRangeMin > dataRangeMax) std::swap(dataRangeMin, dataRangeMax);

    // Create the output vector with default values
    std::vector<int> dataArray(static_cast<size_t>(dataArraySize), 0);

    const int k = sorted_count(dataArraySize, percentageSorted);  // number of sorted elements
    const std::string dir = to_lower(sortingDirection);           // direction: "asc" or "desc"

    if (k > 0) {
        if (dir == "asc") {
            // Start from a small random value near the minimum
            int previousValue = dataRangeMin + (std::rand() % std::min(10, std::max(1, dataRangeMax - dataRangeMin + 1)));
            for (int i = 0; i < k; ++i) {
                int step = (std::rand() % 50) + 1; // random increment between 1 and 50
                int value = previousValue + step;
                // Clamp to avoid going outside [min, max]
                value = clamp_int(value, dataRangeMin, dataRangeMax);
                dataArray[static_cast<size_t>(i)] = value;
                previousValue = value;
            }
        }
        else if (dir == "desc") {
            // Start from a random value near the maximum
            int previousValue = dataRangeMax - (std::rand() % std::min(10, std::max(1, dataRangeMax - dataRangeMin + 1)));
            for (int i = 0; i < k; ++i) {
                int step = (std::rand() % 50) + 1; // random decrement between 1 and 50
                int value = previousValue - step;
                value = clamp_int(value, dataRangeMin, dataRangeMax);
                dataArray[static_cast<size_t>(i)] = value;
                previousValue = value;
            }
        }
    }

    const int span = dataRangeMax - dataRangeMin + 1; // ensure at least 1
    for (int i = (dir == "asc" || dir == "desc") ? k : 0; i < dataArraySize; ++i) {
        int value = dataRangeMin + (std::rand() % span);
        dataArray[static_cast<size_t>(i)] = value;
    }

    return dataArray;
}

std::vector<float> DataGenerator::generateFloat(
    int dataArraySize,
    int dataRangeMin,
    int dataRangeMax,
    float percentageSorted,
    std::string sortingDirection)
{
    seed_once();  // Make sure rand() is initialized

    // Fix invalid arguments
    if (dataArraySize < 0) dataArraySize = 0;
    if (dataRangeMin > dataRangeMax) std::swap(dataRangeMin, dataRangeMax);

    std::vector<float> dataArray(static_cast<size_t>(dataArraySize), 0.0f);

    const int k = sorted_count(dataArraySize, percentageSorted);
    const std::string dir = to_lower(sortingDirection);

    // Simple helper lambda for float randoms in [lo, hi)
    auto frand = [](float lo, float hi) -> float {
        return lo + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (hi - lo);
    };

    if (k > 0) {
        if (dir == "asc") {
            // Start near min and keep increasing slightly
            float previousValue = frand(static_cast<float>(dataRangeMin),
                                        static_cast<float>(std::min(dataRangeMin + 5, dataRangeMax)));
            for (int i = 0; i < k; ++i) {
                float step = frand(0.0f, 20.0f); // 0–20 random step
                float value = previousValue + step;
                // Clamp to range
                value = std::max(static_cast<float>(dataRangeMin),
                                 std::min(value, static_cast<float>(dataRangeMax)));
                dataArray[static_cast<size_t>(i)] = value;
                previousValue = value;
            }
        }
        else if (dir == "desc") {
            // Start near max and keep decreasing slightly
            float previousValue = frand(static_cast<float>(std::max(dataRangeMin, dataRangeMax - 5)),
                                        static_cast<float>(dataRangeMax));
            for (int i = 0; i < k; ++i) {
                float step = frand(0.0f, 20.0f);
                float value = previousValue - step;
                value = std::max(static_cast<float>(dataRangeMin),
                                 std::min(value, static_cast<float>(dataRangeMax)));
                dataArray[static_cast<size_t>(i)] = value;
                previousValue = value;
            }
        }
    }

    for (int i = (dir == "asc" || dir == "desc") ? k : 0; i < dataArraySize; ++i) {
        float value = frand(static_cast<float>(dataRangeMin),
                            static_cast<float>(dataRangeMax));
        dataArray[static_cast<size_t>(i)] = value;
    }

    return dataArray;
}
