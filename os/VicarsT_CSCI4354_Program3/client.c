/********** client.c ********** 
 * Programmer: Timothy Vicars
 * Course: CSCI 4354 
 * Date: February 21, 2019
 *
 * Assignment: Program #3 CPU Scheduler 
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: server.c , client.c, queue.c, queue.h, pcb.h, Makefile
 *
 * Purpose:
 * this client sends "PCB"s to server for queueing and 
 * scheduling.
 *
 * Input: an array of numbers
 *
 * Preconditions:
 *
 * Output: arrival time.
 *
 * Postconditions: 
 *
 * Algorithm:
 * open the common fifo
 * send PCB to server
 * create private fifo
 * open private fifo
 * receive arrival time from fifo
 * close fifo and unlink.
 * exit
 *
 * Estimates    |   Actual
 * design: 30m  |   30m
 * impl:   30m  |   1hr 
 * test:   20m  |   20m
 *
 * EXAMPLE:
 *  ./client 3 4 5 6
 ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "pcb.h"

int main(int argc, char *argv[]) {
    pcb* mypcb = (pcb*)malloc(sizeof(pcb));
    /*this struct holds the burst,arrival
      and fifo name */

    //build the client fifo name
    //and pack it into the struct
    snprintf(mypcb->fifo_name,30,"fifo_to_client%d",getpid());
    //pack the current time into our struct
    mypcb->arrival = 0;
    //create the client fifo
    if ((mkfifo(mypcb->fifo_name, 0666) < 0 && errno != EEXIST)) {
        perror("Can't create fifo file\n");
        return -1;
    }
    /******
     * let's use the arguments of the client program
     * as the burst time, e.g. executing:
     * $ ./client 443
     * will have a burst of 443 time-units.
     * if no argument we will use 10 as burst time.
     *****/
    if (argc < 2) {
        //default if no arguments.
        mypcb->burst = 10;
    }
    else {
        mypcb->burst = atoi(argv[1]);
    }
    // open fifo to server
    char f2sn[] = "fifo_to_server";
    int f2s;
    int f2c;
    if ((f2s = open(f2sn, O_WRONLY)) < 0)
        printf("can't open fifo to server\n");
    // write to the common server fifo
    write(f2s, mypcb, sizeof(*mypcb));
    close(f2s);
    // open the client fifo
    if ((f2c = open(mypcb->fifo_name, O_RDONLY)) < 0)
        printf("can't open fifo to client\n");
    if (read(f2c, mypcb, sizeof(*mypcb)) > 0) {
        printf("arrived: %s %d\n", mypcb->fifo_name, mypcb->arrival);
    }
            
    printf("\nDone!\n");

    //close fifo
    close(f2c);
    //unlink client fifo
    unlink(mypcb->fifo_name);
    return 0;
}
