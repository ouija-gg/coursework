/*
     a. PROGRAMMER: Timothy Vicars 

     b. COURSE:  CSCI 3321 Numerical Methods 

     c. DATE:   Feb 10, 2019

     d. ASSIGNMENT:  Number 1

     e. ENVIRONMENT: Arch Linux, Clang/GCC C++

     f. FILES INCLUDED: hw1.cpp

     g. PURPOSE:  this program attempts to showcase the effects of rounding errors while
                    while approximating a derivative function.

     h. INPUT:  We will use a function f'(x) ≃ (f(x+h) - f(x)) / h

     i. PRECONDITIONS:  

     j. OUTPUT:  this output is a listing of errors at each `h`

     k. POSTCONDITIONS: 

     l. ALGORITHM:  f'(x) ≃ (f(x+h) - f(x)) / h

     m.  ERRORS:  

     n.  EXAMPLE: ./hw2

*/
#include <iostream>
#include <iomanip>
#include <cmath>

double fx(double x) {
    return (sin(pow(x,4)) + pow(x,2));
}

int main() {
    //approx our fx in a loop, looping on `h`
    double f_prime;
    double x = 2.0;
    for (double h = 1; h > 2e-12; h/=2) {
        f_prime = (fx(x+h) - fx(x))/ h;
        std::cout << "F'(x) = " << std::setw(10) << f_prime << std::setw(15) << "\t|| Error (h=" 
            <<  h << ") : " << std::setw(10) 
            << f_prime-(4 + 32 * cos(16)) << std::endl;
    }
    return 0;
}
// compile:  g++ -o hw1 hw1.cpp
// execute:  ./hw1
