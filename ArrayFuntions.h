

#ifndef ARRAYFUNTIONS_H
#define ARRAYFUNTIONS_H

#include <vector>
template <typename T>
concept NumericType2= std::integral<T> || std::floating_point<T>; // limits the use to int and float types
class ArrayFuntions {
    public:
    template <NumericType2 T>
    static void displayArray(std::vector<T>);

    template <NumericType2 T>
    static bool sortedAscending(std::vector<T> arr);
};


#include "ArrayFuntions.tpp"

#endif //ARRAYFUNTIONS_H
