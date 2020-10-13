/********** server.c **********
 * Programmer: Timothy Vicars
 * Course: CSCI 4354
 * Date: February 21, 2019
 *
 * Assignment: Program #2 Server summation
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: server.c , client.c
 *
 * Purpose:
 * This program recieves an array, name, pid and array size from
 * a client, creates a private fifo for the client and sends
 * back a summation of the array through the private fifo.
 *
 * Input: array of numbers, size of array, PID of client
 *
 * Preconditions:
 *
 * Output: the sum of each array, and a total average of
 *  every array.
 *
 * Postconditions: 
 *
 * Algorithm:
 *   create common server fifo.
 *   read client pid from serverfifo.
 *   read job name, array, and size from serverfifo
 *   create client fifo based on PID
 *   sum the array
 *   open client pid fifo
 *   write array sum to client via pid fifo
 *   close the FIFOs
 *   unlink the FIFOs
 *   average and all client arrays and print the average.
 *
 * Estimates    |   Actual
 * design: 30m  |   30m
 * impl:   1hr  |   1hr
 * test:   20m  |   30m
 ***************************/

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "job.h"

int main() {
    int total = 0;
    int totalnumbers = 0;
    //create and open common server fifo
    char f2sn[] = "fifo_to_server";
    int f2s;
    if ((mkfifo(f2sn,0666) < 0 && errno != EEXIST)) {
        perror("Can't create server fifo\n");
        return -1;
    }
    if ((f2s = open(f2sn, O_RDONLY)) < 0)
        printf("Can't open server fifo\n");

    //only handle 3 clients for now
    for(int clients = 0; clients < 3;) {
        // read the client
        // and open the private fifo
        // and calc the summation
        // and write to client
        int sum = 0;
        job clientjob;
        if (read(f2s,&clientjob,sizeof clientjob)) {
            printf("name: %s, ", clientjob.name);
            for (int i = 0; i < clientjob.size; i++) {
                printf("%d ", clientjob.array[i]);
                sum += clientjob.array[i];
            }
            printf(" = %d\n", sum);
            //open the client fifo
            char f2cn[20];
            sprintf(f2cn, "fifo_to_client%d", clientjob.pid);
            int f2c = open(f2cn, O_WRONLY);
            //write the summation and close
            write(f2c, &sum, sizeof sum);
            close(f2c);
            total += sum;
            totalnumbers += clientjob.size;
            clients++;
        }
    }
    //close and unlink fifo for server
    close(f2s);
    unlink(f2sn);
    printf("Totals from all clients:  %d , Average: %.2f\n", total, (double) total/totalnumbers);
    return 0;
}
