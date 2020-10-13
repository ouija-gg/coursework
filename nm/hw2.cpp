/*
     a. PROGRAMMER: Timothy Vicars 

     b. COURSE:  CSCI 3321 Numerical Methods 

     c. DATE:   Feb 20, 2019

     d. ASSIGNMENT:  Number 2

     e. ENVIRONMENT: Arch Linux, Clang/GCC C++

     f. FILES INCLUDED: hw1.cpp

     g. PURPOSE:  this program approximates the root of a function using the 
                 secant method of approximation.

     h. INPUT: we will use the function FX(X) = cos(x^2 /4) - 1/3

     i. PRECONDITIONS:  

     j. OUTPUT:  

     k. POSTCONDITIONS: 

     l. ALGORITHM: 
        x2 = x1 - ((x0 - x1)f(x1)) / (f(x0)-f(x1))

     m.  ERRORS:  

     n.  EXAMPLE: ./hw2

*/
#include <stdio.h>
#include <math.h>

/* we choose c++ because math operations
 * are overloaded for doubles and simpler
 * to read
 */
double fx(double x) {
    return (cos(pow(x,2)/4.0f) -  1.0f/3.0f);
}
double approx(double x1, double x0) {
    printf("x: %.12f\n", x0);
    return x1 - (((x0-x1) * fx(x1))/
            (fx(x0) - fx(x1)));
}

int main() {
    //we set our first approximations
    double xn_1 = 3.0;
    double xn = 2.0;
    //we want to track the # of iterations
    int iter = 0;
    while (abs(fx(xn)) > 0.000000001) {
        double temp = approx(xn, xn_1);
        xn_1 = xn;
        xn = temp;
        iter++;
    }
    printf("fx(x=%.12lf) = %.12lf\n", xn, fx(xn));
    printf("with %d iterations.\n", iter);
    return 0;
}
// compile:  g++ -o hw2 hw2.cpp
// execute:  ./hw2
