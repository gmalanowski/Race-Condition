#ifndef ARRAYFUNTIONS_TPP
#define ARRAYFUNTIONS_TPP

#include "ArrayFuntions.h"
#include <iostream>
using namespace std;
template <NumericType2 T>
void ArrayFuntions::displayArray(vector<T> arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
}
template<NumericType2 T>
bool ArrayFuntions::sortedAscending(vector<T> arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}
#endif // ARRAYFUNTIONS_TPP