/********** server.c **********
 * Programmer: Timothy Vicars
 * Course: CSCI 3333
 * Date: February 12, 2019
 *
 * Assignment: Program #1 Server calculator
 *
 * Environment: Arch Linux, GCC/Clang compiler
 *
 * Files Included: server.c , client.c
 *
 * Purpose:
 * the purpose of this program is to use FIFO files as a medium between a client
 * and server, acting as a simple calculator.
 *
 * Input: 2 integers and 1 operator.
 *
 * Preconditions: only integers and operators (+ - / * )
 *
 * Output: the equation, and the operation evaluation.
 *
 * Postconditions: 
 *
 * Algorithm:
 * read fifo from the client
 * consume the operands and operator
 * perform the correct calculation.
 * write the evaluations to client by different fifo
 * close/unlink the FIFOs
 *
 * Estimates    |   Actual
 * design: 30m  |   20m
 * impl:   1hr  |   30m
 * test:   20m  |   20m
 ***************************/

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "equation.h"

int main() {
    int f2s, f2c; // fifo handles
    equation equ; // or data struct

    //create FIFOs for read and write
    if ((mkfifo("fifo_to_server", 0666) < 0 && errno != EEXIST)) {
        perror("can't create server fifo\n");
        return -1;
    }
    if ((mkfifo("fifo_to_client", 0666) < 0 && errno != EEXIST)) {
        perror("can't create client fifo\n");
        return -1;
    }
    //open the fifos
    if ((f2s = open("fifo_to_server", O_RDONLY)) < 0)
        printf("can't open server fifo\n");
    if ((f2c = open("fifo_to_client", O_WRONLY)) < 0)
        printf("can't open client fifo\n");

    // read the fifo data into struct;
    read(f2s, &equ, sizeof(equ));

    //determine operator and calculate
    int result;
    switch(equ.operator) {
        case '+':
            result = equ.op1+equ.op2;
            break;
        case '-':
            result = equ.op1 - equ.op2;
            break;
        case '*':
            result = equ.op1*equ.op2;
            break;
        case '/':
            result = equ.op1/equ.op2;
            break;
    }
    printf("equation: %d %c %d = %d\n",equ.op1,equ.operator,equ.op2,result);
    //write the data out to fifo
    write(f2c, &result, sizeof(result));

    //close and unlink fifos
    close(f2s);
    close(f2c);
    unlink("fifo_to_server");
    unlink("fifo_to_client");
    return 0;
}
