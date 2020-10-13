/********** server.c **********
 * {{{
 * Programmer: Timothy Vicars
 * Course: CSCI 4354
 * Date: April 16, 2019
 *
 * Assignment: Program #4 Memory Paging
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: server.c , client.c, pcb.h,
 *      queue.c, queue.h
 *
 * Purpose:
 * This program creates a server that takes clients' memory requests,
 * queues, and allocates memory pages and frames for them.
 *
 * Input: a "PCB" data structure
 *        with burst time
 *        and memory requirements
 *
 * Preconditions: the server will only run 5 clients for now
 *
 * Output:
 * completion time, frame numbers, amount of fragmentation.
 * or an error messages
 *
 * Postconditions:
 *
 * Algorithm:
 * loop till 5 clients finished
 *      read fifo for new clients
 *      if client-memory fits
 *          enqueue client
 *      if no job running
 *          dequeue new job
 *      run job
 *      if job finished
 *          destroy job
 *          pull new job, or wait
 *
 * Estimates    |   Actual
 * design: 1hr  |
 * impl:   1hr  |
 * test:   1hr  |
 *
 *  COMPILE: gcc -std=C99 -o server server.c queue.c
 *  or use : make
 *  RUN:     ./server
 *  }}}
 ***************************/
#include "equation.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define framesize 10
#define numframes 10

int main() {
    // create the server fifo
    const char f2sn[] = "fifo_to_server";
    if (mkfifo(f2sn, 0666) < 0) {
        perror("Can't create server fifo\n");
        return -1;
    }
    // open the server fifo
    int f2s = open(f2sn, O_RDONLY);
    if (f2s < 0) {
        perror("Can't open server fifo.\n");
        return -1;
    }
    int memfull = 0;
    queue myqueue = { NULL, NULL };
    mem_req newreq;
    // main loop
    while (!memfull) {
        // read new requests
        while (read(f2s, newreq, sizeof(newreq)) > 0) {
            // add them to queue
            enqueue(&myqueue, &newreq);
        }
        while (myqueue.head != NULL) {
            // check memory and dequeue
        }
    }
    // once our memory is full we reject new clients

    // close server fifo and unlink it
    close(f2s);
    unlink(f2sn);
    return 0;
}
