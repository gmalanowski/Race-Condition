#ifndef ARRAYFUNCTIONS_TPP
#define ARRAYFUNCTIONS_TPP

#include "ArrayFunctions.h"
#include <iostream>

template <NumericType2 T>
void ArrayFunctions::displayArray(const std::vector<T>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }
}

template<NumericType2 T>
bool ArrayFunctions::sortedAscending(const std::vector<T>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}
#endif // ARRAYFUNCTIONS_TPP