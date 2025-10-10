//
// Created by Damian on 23-Apr-25.
//

#ifndef CSVFILEHANDLER_H
#define CSVFILEHANDLER_H


#include <string>

class CsvFileHandler {
    public:
    static void saveTo(std::string csvName, double time1, bool is_sorted1, double time2, bool is_sorted2,  int nr);
    static void beginCSV(std::string csvName,int size);
};



#endif //CSVFILEHANDLER_H
