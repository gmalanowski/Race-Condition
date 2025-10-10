cmake_minimum_required(VERSION 3.30)
project(MergeSortRaceCondition)

set(CMAKE_CXX_STANDARD 20)

add_executable(MergeSortRaceCondition main.cpp
        ThreadPool.cpp
        ThreadPool.h
        DataGenerator.cpp
        DataGenerator.h
        CsvFileHandler.cpp
        CsvFileHandler.h
        ArrayFuntions.tpp
        ArrayFuntions.h
)
