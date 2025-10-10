#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H
#include <vector>
#include <string>
using namespace std;

class DataGenerator {
public:
    static vector<int> generateInt(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, string sortingDirection);
    static vector<float> generateFloat(int dataArraySize, int dataRangeMin, int dataRangeMax, float percentageSorted, string sortingDirection);

};

#endif // DATAGENERATOR_H
