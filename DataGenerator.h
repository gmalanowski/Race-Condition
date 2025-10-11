#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
#include <string>

class DataGenerator {
public:
    static std::vector<int> generateInt(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, std::string sortingDirection);
    static std::vector<float> generateFloat(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, std::string sortingDirection);

};

#endif // DATAGENERATOR_H
