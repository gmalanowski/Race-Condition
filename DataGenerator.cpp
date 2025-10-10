#include "DataGenerator.h"
#include <cstdlib>
#include <ctime>


using namespace std;

vector<int> DataGenerator::generateInt(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, string sortingDirection) {
    vector<int> dataArray(dataArraySize);

    int sortedTableSize = static_cast<int>(dataArraySize * percentageSorted);
    if (sortingDirection == "asc") {
        int previousValue = dataRangeMin + rand() % 10; // start within range
        for (int i = 0; i < sortedTableSize; i++) {
            int step = rand() % 50 + 1; // varied step
            int value = previousValue + step;
            dataArray[i] = value;
            previousValue = value;
        }
    }
    else if (sortingDirection == "desc") {
        int previousValue = dataRangeMax - rand() % 10;
        for (int i = 0; i < sortedTableSize; i++) {
            int step = rand() % 50 + 1;
            int value = previousValue - step;
            dataArray[i] = value;
            previousValue = value;
        }
    }

    // Fill the rest with random values
    for (int i = sortedTableSize; i < dataArraySize; i++) {
        int value = dataRangeMin + rand() % (dataRangeMax - dataRangeMin + 1);
        dataArray[i] = value;
    }

    return dataArray;
}

vector<float> DataGenerator::generateFloat(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, string sortingDirection) {
    vector<float> dataArray(dataArraySize);

    int sortedTableSize = static_cast<int>(dataArraySize * percentageSorted); // static_cast is better than C-style here C-style gave some errors
    if (sortingDirection == "asc") {
        float previousValue = dataRangeMin + static_cast<float>(rand()) / RAND_MAX * 5.0f;
        for (int i = 0; i < sortedTableSize; i++) {
            float step = static_cast<float>(rand()) / RAND_MAX * 20.0f;
            float value = previousValue + step;
            dataArray[i] = value;
            previousValue = value;
        }
    }
    else if (sortingDirection == "desc") {
        float previousValue = dataRangeMax - static_cast<float>(rand()) / RAND_MAX * 5.0f;
        for (int i = 0; i < sortedTableSize; i++) {
            float step = static_cast<float>(rand()) / RAND_MAX * 20.0f;
            float value = previousValue - step;
            dataArray[i] = value;
            previousValue = value;
        }
    }

    // Fill the rest with random values
    for (int i = sortedTableSize; i < dataArraySize; i++) {
        float value = dataRangeMin + static_cast<float>(rand()) / RAND_MAX * (dataRangeMax - dataRangeMin);
        dataArray[i] = value;
    }

    return dataArray;
}
