#ifndef ARRAYFUNCTIONS_H
#define ARRAYFUNCTIONS_H

#include <vector>
#include <concepts>

template <typename T>
concept NumericType2= std::integral<T> || std::floating_point<T>; // limits the use to int and float types
class ArrayFunctions {
    public:
    template <NumericType2 T>
    static void displayArray(const std::vector<T>&);

    template <NumericType2 T>
    [[nodiscard]] static bool sortedAscending(const std::vector<T>& arr);
};


#include "ArrayFunctions.tpp"

#endif //ARRAYFUNCTIONS_H
