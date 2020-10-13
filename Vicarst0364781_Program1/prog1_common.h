#include <algorithm>
#include <iostream>
#include <string.h>

void print_array(int* array, size_t size) {
    using std::cout;

    cout << "{ ";
    for (int i = 0; i < size; i++) {
        std::cout << array[i];
        if (i == size - 1)
            cout << " }" << std::endl;
        else
            cout << ", ";
    }
}

void fill_array(int* array) {
    int prefill[] = {
        4,  2,  3,  5,  7,  // 5
        19, 20, 7,  3,  1,  // 10
        22, 50, 10, 28, 9,  // 15
        8,  90, 4,  6,  9,  // 20
        15, 23, 89, 9,  55, // 25
        23, 28, 49, 29, 19  // 30
    };
    memcpy(array, &prefill, sizeof(prefill));
}
