#ifndef CSVFILEHANDLER_H
#define CSVFILEHANDLER_H

#include <string>

class CsvFileHandler {
public:
    static void beginCSV(const std::string& csvName, int size);

    static void saveTo(const std::string& csvName,
                       double serialTime, bool serialSorted,
                       double parallelTime, bool parallelSorted,
                       int numThreads, int raceValue, int safeValue, int arraySize);
};

#endif // CSVFILEHANDLER_H
