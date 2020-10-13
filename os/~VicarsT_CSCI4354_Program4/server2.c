/********** server.c **********
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
 ***************************/

#include "pcb.h"
#include "queue.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {
    // create the common fifo
    char f2sn[] = "fifo_to_server";
    if ((mkfifo(f2sn, 0666) < 0 && errno != EEXIST)) {
        perror("Can't create server fifo\n");
        return -1;
    }
    // open common fifo
    int f2s;
    if ((f2s = open(f2sn, O_RDONLY)) < 0)
        printf("Can't open server fifo\n");

    // create the queue as empty.
    queue myq = { NULL, NULL };

    int running = 1;
    int nclients = 0;
    int firstjobtime = 0;
    int tatime = 0;
    int jobnumber = 0;
    int wait = 0;

    // MAIN LOOP
    // this runs the timer, reads the fifo,
    // and enqueues new PCBs
    pcb* currentjob = NULL;
    for (int timer = 0; running; timer++) {
        pcb* clientpcb = (pcb*)malloc(sizeof(pcb));
    pulljob: // gotos are not that scary for state machines
        if (read(f2s, clientpcb, sizeof(pcb)) > 0) {
            // if the first job, restart timer
            if (nclients == 0) firstjobtime = timer = 0;
            nclients++;
            printf("j%d@%d added.\n", nclients, timer);
            // record arrival time for calculating wait
            // and turnaround time
            clientpcb->arrival = timer;
            clientpcb->state = Wait;
            enqueue(&myq, clientpcb);
            int f2c = open(clientpcb->fifo_name, O_WRONLY);

            // we have to write the client file
            // here otherwise it lags and bugs the server
            // for some reason. it is really hard to debug
            write(f2c, clientpcb, sizeof(*clientpcb));
            close(f2c);
        }
        if ((currentjob == NULL) && (myq.head != NULL)) {
            // we need to dequeue and start jobs
            currentjob = (pcb*)malloc(sizeof(pcb));
            currentjob = dequeue(&myq);
            jobnumber++;
            printf("j%d@%d started.\n", jobnumber, timer);
            wait += timer - (currentjob->arrival);
        }
        if (currentjob != NULL) {
            currentjob->burst--;
            printf(".");
            fflush(stdout);
            if (currentjob->burst < 0) {
                // job has finished
                tatime += timer - currentjob->arrival;
                printf("j%d@%d done.\n", jobnumber, timer);
                free(currentjob);
                currentjob = NULL;
                // stop running after 5th job
                // finishes
                if (jobnumber >= 5) running = 0;
                goto pulljob; // pull new job
                // use goto because i can't find a graceful way
                // to do this yet.
            }
        }
        usleep(timeunits);
    }
    // close and unlink fifo for server
    close(f2s);
    unlink(f2sn);
    printf("I'm done!\n");
    float waitavg = 0;
    printf("Throughput: %dclients\t\n\
            turnaround:%d\n\
            avg wait time: %.2f\n",
           nclients, tatime / nclients, (float)wait / nclients);
    return 0;
}
