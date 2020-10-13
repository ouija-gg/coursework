#include <stdio.h>
#include <stdlib.h>
/****
 * Author: Timothy Vicars
 * Purpose: this program adds 2x 5-element arrays and displays them
    and their summation.
****/


//display array function
//with semi-pretty printing.
void displayArray(int arry[], size_t arSize) {
   printf("[ ");
   for (int i =0; i<arSize; i++) {
       printf("%5d",arry[i]);
       if (i<arSize-1)
           printf(",");
   }
   printf(" ]");
}

int main() {
    //create first 5 element array
    int arrayA[] = { 3, 4, 5, 6, 7};
    //create second array
    int arrayB[] = { 100, 200, 300, 400, 500 };

    //the summation array
    int sum[5];

    // print array A and B
    printf("     ");
    displayArray(arrayA,5);
    printf("\n  +  ");
    displayArray(arrayB,5);
    printf("\n");

    // finally sum our arrays into sum[]
    for (int i = 0; i < 5; i++)
        sum[i] = arrayA[i] + arrayB[i];
    //display sum
    printf("  =  ");
    displayArray(sum,5);    
    printf("\n");

    return 0;
}

// compile:
// gcc -o hw2 hw2.c
// execute:
// ./hw2
