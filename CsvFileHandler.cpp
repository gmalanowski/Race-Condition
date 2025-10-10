//
// Created by Damian on 23-Apr-25.
//

#include "CsvFileHandler.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
void CsvFileHandler::beginCSV(string csvName, int size) {
    string name = "nr";
    string name1 = "Serial";
    string name2 = "Serial Sorted Correctly";
    string name3 = "Parallel";
    string name4 = "Parallel Sorted Correctly";


    ofstream file(csvName, ios::app);

    file <<name<<","<< name1 <<","<< name2 <<","<< name3 <<","<< name4 << ",Size: ," << size <<endl;

    file.close();

}
void CsvFileHandler::saveTo(string csvName, double time1, bool is_sorted1, double time2, bool is_sorted2, int nr) {
    ofstream file(csvName, ios::app);// write at the end of file

    file <<nr<<","<< time1 <<","<< is_sorted1 <<","<< time2 <<","<< is_sorted2 << endl;

    file.close();
}