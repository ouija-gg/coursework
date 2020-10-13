/********** hw9.c **********
 * Programmer: Timothy Vicars
 * Course: CSCI 4354
 * Date: May 1, 2019
 *
 * Assignment: Homework #4 Threads
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: hw9.c
 *
 * Purpose:
 * This program creates several threads to manipulate
 * 2 arrays at the same time.
 *
 * Input: an empty array and size, supplied at main function.
 *
 * Preconditions:
 *
 * Output: The array of Ints and array of Chars
 *
 * Postconditions:
 *
 * Algorithm:
 * thread0: write 0 to array
 *          on every odd element.
 * thread1: write 1 to array
 *          on every even element.
 * thread5a:write aaaaa to array
 *          skipping 5, start at 0
 * threadba:write bbbbb to array
 *          skipping 5, start at 5
 *
 * main: create a thread handle for each thread function
 *       execute the threads
 *       print the arrays.
 *       exit
 *
 * Estimates    |   Actual
 * design: 30m  |   30m
 * impl:   20m  |   30m
 * test:   10m  |   10m
 *
 * COMPILE: gcc -o hw9 hw9.c -lpthread
 * EXEC:    ./hw9
 ***************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this struct holds our
 * thread arguments */
typedef struct thread_args {
    size_t iarSize;
    size_t carSize;
    int* iarray;
    char* carray;
} args;

/* fills 0 , 2 , 4 , 6 , etc with 1 */
void* thread0(args* myargs) {
    int i;
    for (i = 0; i < myargs->iarSize; i += 2) myargs->iarray[i] = 0;
}
/* fills 1 , 3 , 5 , 7 , etc with 0 */
void* thread1(args* myargs) {
    int i;
    for (i = 1; i < myargs->iarSize; i += 2) myargs->iarray[i] = 1;
}
/* fills array [0-4, 10-14, etc] with aaaaa */
void* thread5a(args* myargs) {
    int i;
    for (i = 0; i < myargs->carSize; i += 10)
        memcpy(myargs->carray + i, "aaaaa", 5);
}
/* fills array [5-9, 15-19, etc] with bbbbb */
void* thread5b(args* myargs) {
    int i;
    for (i = 5; i < myargs->carSize; i += 10)
        memcpy(myargs->carray + i, "bbbbb", 5);
}
void printArgs(args* myargs) {
    /* print the int array */
    printf("intArray  = { ");
    int i = 0;
    for (i = 0; i < myargs->iarSize; i++) {
        printf("%d", myargs->iarray[i]);
        /* don't print comma on last print */
        if (i < myargs->iarSize - 1)
            printf(",");
    }
    /* close the array */
    printf(" }\nCharArray = { ");
    /* print the char array */
    for (i = 0; i < myargs->carSize; i++) {
        printf("%c", myargs->carray[i]);
        /* don't print comma on last print */
        if (i < myargs->carSize - 1)
            printf(",");
    }
    /* close the array */
    printf(" }\n");
}
/* MAIN */
int main() {
    int* intarray = malloc(sizeof(int) * 20);
    char* chararray = malloc(sizeof(char) * 30);
    /* pack the struct to pass to thread functions */
    args myargs = {
        20,
        30,
        intarray,
        chararray,
    };

    pthread_t one, two, three, four;

    pthread_create(&one, NULL, (void*)thread0, &myargs);
    pthread_create(&two, NULL, (void*)thread1, &myargs);
    pthread_create(&three, NULL, (void*)thread5a, &myargs);
    pthread_create(&four, NULL, (void*)thread5b, &myargs);

    pthread_join(one, NULL);
    pthread_join(two, NULL);
    pthread_join(three, NULL);
    pthread_join(four, NULL);

    printArgs(&myargs);

    return 0;
}
