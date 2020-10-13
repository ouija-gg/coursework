/********** client.c ********** 
 * Programmer: Timothy Vicars
 * Course: CSCI 4354 
 * Date: April 16, 2019
 *
 * Assignment: Program #4 Memory Paging 
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
 * recieve pcbinfo, errorcode
 * close fifo and unlink.
 * exit
 *
 * Estimates    |   Actual
 * design: 30m  |   
 * impl:   30m  |   
 * test:   20m  |   
 *
 * EXAMPLE:
 *  ./client <burst> <mem>
 *  ./client 14 23
 ***************************/

#include "common.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    mem_req myreq;
    /***
     * use arguments as burst and mem request size
     * like ./client <burst> <mem_size>
     * e.g. ./client 14 24
     ***/
    if (argc < 3) {
        printf("Error: Invalid arguements\nSupply arguments like ./client <burst> <mem>\n");
        return -1;
    }
    else {
        // pack arguments into mem_req struct
        myreq.burst = atoi(argv[1]);
        myreq.req_size = atoi(argv[2]);
    }
    // open server fifo
    int f2s = open("fifo_to_server", O_WRONLY);
    if (f2s < 0) {
        printf("Can't open server fifo\n");
        return -1;
    }
    // write to server and close fifo
    write(f2s, &myreq, sizeof(myreq));
    close(f2s);

    // string build the private fifo name, create and open it
    snprintf(myreq.fifo_name, 30, "fifo_to_client%d", getpid());
    if (mkfifo(myreq.fifo_name, 0666) < 0) {
        perror("Can't create fifo file\n");
        return -1;
    }
    // read the private fifo, close and unlink it
    int f2c = open(myreq.fifo_name, O_RDONLY);
    if (f2c < 0)
        printf("can't open %s\n", myreq.fifo_name);
    if (read(f2c, &myreq, sizeof(myreq)) > 0)
        printf("%s: queued\n");
    close(f2c);
    unlink(myreq.fifo_name);
    return 0;
}

