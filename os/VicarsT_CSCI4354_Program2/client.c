/********** client.c ********** 
 * Programmer: Timothy Vicars
 * Course: CSCI 4354 
 * Date: February 21, 2019
 *
 * Assignment: Program #2 Server calculator
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: server.c , client.c, jobs.h, makefile
 *
 * Purpose:
 * the purpose of this program is to use FIFO files as 
 * a medium between a client
 * and server, acting as a simple calculator.
 *
 * Input: an array of numbers
 *
 * Preconditions:
 *
 * Output: the summation of the array and the process ID
 *
 * Postconditions: 
 *
 * Algorithm:
 *   open common server fifo.
 *   write client pid, array of ints and arraysize
 *     to server fifo.
 *   create and open private client fifo made from process ID.
 *   read summation from private fifo.
 *   print summation.
 *   close private fifo
 *   unlink private fifo
 *   close server fifo.
 *   exit
 *
 * Estimates    |   Actual
 * design: 30m  |   30m
 * impl:   1hr  |   1hr 
 * test:   20m  |   20m
 *
 * EXAMPLE:
 *  ./client 3 4 5 6
 ***************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "job.h"

void printArray(int array[], size_t size) {
        printf("{ ");
        for (int i=0;i<size-1;i++)
            printf("%d, ",array[i]);
        printf("%d }\n", array[size-1]);
}

int main(int argc, char *argv[]) {
    job myjob; //the struct
    myjob.pid = getpid();
    //build the client fifo name and job name
    char f2cn[20];
    sprintf(f2cn,"fifo_to_client%d",getpid());
    sprintf(myjob.name, "Job%d", getpid());
    //create the client fifo
    if ((mkfifo(f2cn, 0666) < 0 && errno != EEXIST)) {
        perror("Can't create fifo file\n");
        return -1;
    }

    /*******
     * this packs the arguments into the array.
     * if no arguments are supplied, then it uses default
     * values of 4 ,5 ,7
    *******/
    if (argc < 2) {
        int defarray[3] = { 4, 5 , 7 };
        memcpy(&myjob.array, defarray, sizeof defarray);
        myjob.size = 3;
    }
    else {
        for (int i=0;i < argc-1;i++)
            myjob.array[i] = atoi(argv[i+1]);
        myjob.size = argc - 1;
    }
    //displays the array
    printf("Array is: ");
    printArray(myjob.array,myjob.size);
    printf("\n");

    // open fifo to server
    char f2sn[] = "fifo_to_server";
    int f2s;
    int f2c;
    if ((f2s = open(f2sn, O_WRONLY)) < 0)
        printf("can't open fifo to server\n");
    // write to the common server fifo
    write(f2s, &myjob, sizeof myjob);
    // open the client fifo
    if ((f2c = open(f2cn, O_RDONLY)) < 0)
        printf("can't open fifo to client\n");
    //read summation from server via private fifo
    int sum;
    read(f2c, &sum, sizeof sum);

    printf("Sum from fifo: %d\n", sum);

    //close fifos
    close(f2s);
    close(f2c);
    //unlink client fifo
    unlink(f2cn);
    return 0;
}
