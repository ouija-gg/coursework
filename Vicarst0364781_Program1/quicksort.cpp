#include "prog1_common.h"
#include <algorithm>
#include <iostream>

using std::cout;

int partition(int* array, int low, int high) {
    // create a pivot at the high point
    int pivot = array[high];
    int i = low;

    // iterate the array
    for (int j = low; j <= high; j++) {
        if (array[j] < pivot) {
            std::swap(array[i], array[j]);
            i++;
        }
    }
    std::swap(array[i], array[high]);
    return i;
}
void quicksort(int* array, int low, int high) {
    if (low < high) {
        int part = partition(array, low, high);
        // split the array into 2 parts and sort
        // both sides on either side of the pivot
        quicksort(array, low, part - 1);
        quicksort(array, part + 1, high);
    }
}

int main() {
    int myarray[30];
    fill_array(myarray);
    cout << "Unsorted: " << std::endl;
    print_array(myarray, 30);

    quicksort(myarray, 0, 29);

    cout << "Sorted: " << std::endl;
    print_array(myarray, 30);
    return 0;
}
