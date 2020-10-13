#include "prog1_common.h"
#include <iostream>

using std::cout;
using std::endl;

void heapify(int array[], int n, int i) {
    int biggest = i; // assume root is biggest
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // is left child bigger than root?
    // then make the biggest the child.
    if ((left < n) && (array[left] > array[biggest]))
        biggest = left;

    // is right child bigger than root?
    // then make the biggest the child.
    if ((right < n) && (array[right] > array[biggest]))
        biggest = right;

    // is the root the largest?
    if (biggest != i) {
        std::swap(array[i], array[biggest]);

        // recursive heapify
        heapify(array, n, biggest);
    }
}

void heapsort(int array[], int n) {
    // heapify the first half of the array
    for (int i = n / 2 - 1; i >= 0; i--) heapify(array, n, i);

    for (int i = n - 1; i >= 0; i--) {
        std::swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}
int main() {
    int myarray[30];
    fill_array(myarray);
    cout << "Unsorted: " << endl;
    print_array(myarray, 30);

    cout << "Sorted: " << endl;
    heapsort(myarray, 30);
    print_array(myarray, 30);
}
