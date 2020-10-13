/********** client.c ********** 
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
 * read 2 integers and 1 operand from the user 
 * pack the inputs into a struct
 * write the struct to the server via a FIFO
 * open the FIFO from the server
 * read the calculation from the server fifo
 * close/unlink the FIFOs
 *
 * Estimates    |   Actual
 * design: 30m  |   20m
 * impl:   1hr  |   1hr
 * test:   20m  |   20m
 ***************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "equation.h"

int main() {
    int f2s; //fifo to server
    int f2c; //fifo to client 
    equation equ;

    //make sure we can open the FIFO
    if ((f2s = open("fifo_to_server", O_WRONLY)) < 0)
        printf("Can't open FIFO to write\n");
    if ((f2c = open("fifo_to_client", O_RDONLY)) < 0)
        printf("Can't open FIFO to read\n");

    //get the operator to use
    for (bool i = true; i;) {
        printf("Enter a mathematical operator: ");
        char op;
        scanf(" %c",&op);
        //make sure it is a real operator
        // eg. * + - /
        switch(op) {
            case '+':
            case '-':
            case '*':
            case '/':
                i = false;
                break;
            default:
                break;
        } 
        //put the op in the struct
        equ.operator = op;
    }

    //now get the numbers
    //and put them in the struct
    printf("Enter a number1: ");
    scanf(" %d",&(equ.op1));
    printf("Enter a number2: ");
    scanf(" %d",&(equ.op2));

    //write to server fifo
    write(f2s, &equ, sizeof(equ));
    printf("Equation:  %d %c %d = \n", equ.op1,equ.operator,equ.op2);

    //read the result
    int result;
    read(f2c, &result, sizeof(result));
    printf("Result is:   %d\n",result);
    
    // close and unlink fifos
    close(f2c);
    close(f2s);
    unlink("fifo_to_server");
    unlink("fifo_to_client");
    return 0;
}
