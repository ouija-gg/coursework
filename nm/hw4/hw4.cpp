/*
     a. PROGRAMMER: Timothy Vicars 

     b. COURSE:  CSCI 3321 Numerical Methods 

     c. DATE:   March 31, 2019

     d. ASSIGNMENT:  Number 4

     e. ENVIRONMENT: Arch Linux, Clang/GCC C++

     f. FILES INCLUDED: hw4.cpp

     g. PURPOSE:  
        this is a variant of the classical runge-kutta 4th order
        method.

     h. INPUT: 

     i. PRECONDITIONS:  

     j. OUTPUT:  
        this program approximates an integral by the runge-kutta method.
        it also displays a few T and X values for each pass with differing
        H values.

     k. POSTCONDITIONS: 

     l. ALGORITHM: 
        Runge-Kutta:
        x(t+h) = x(t) + 1/6 * (k1 + 2k2 + 2k3 + k4)
        where k1 = h*f(t,x)
              k2 = h*f(t + h/2, x + k1/2)
              k3 = h*f(t + h/2, x + k2/2)
              k4 = h*f(t + h, x + k3)

     m.  ERRORS:  

     n.  EXAMPLE: ./hw4

     x.  NOTES:
        obviously, the smaller the step-size (h)
        the more accurate the approximation becomes.

*/
#include <stdio.h>
#include <cmath>
#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <wchar.h>

/* we choose c++ because math operations
 * are overloaded for doubles and simpler
 * to read
 */

//this is important when using function pointers;
//otherwise the syntax becomes even stupider
//and more cumbersome.
typedef double (*func_dd)(double,double);

using std::string;

// forward declare printer func
void pprint_head(const string& head);

//the problem to be solved
double fx(double x, double t) {
    return pow(x,2) * cos(5*t);
}
double rungekutta(func_dd ff, double h, double xinit, const double tmin, const double tmax) {
    auto printer = [](double x, double t) {
        //lambda for printing
        printf("|   t=%.20f\tx=%.20f   |\n",t,x);
    };
    // the initial value is the first x value
    double x = xinit;
    double lastx = xinit;

    //create a t vector of correct size
    //example: let t = { 0.0 , 2 }
    //     h = 0.1,    size = 20
    //     h = 0.025,  size = 40
    //     h = 0.0125, size = 80
    //     etc.
    std::vector<double> t(int((tmax-tmin)/h)+1 );
    printer(x, tmin); //print the first value
    for (int j = 0; j < t.size(); j++) {
        //fill the t vector
        t[j] = (j+1)*h;

        /* classical runge-kutta uses 
         * k2 = hf(x+k1/2, t+h/2)
         * and k3 = hf(x+k2/2, t+h/2)
         *
         * But we have to try a variant so we use
         * k2 = hf(x + k1/3, t+h/3)
         * k3 = hf(x + k1/3, t+h/3)
         * and make x(t + h) = (k1 + 3k2 + 3k3 + k4)/8
         */
        double k1 = h*(ff)(x, t[j]);
        double k2 = h*(ff)(x+(k1/3.0), t[j]+(h/3.0));
        double k3 = h*(ff)(x+(k2/3.0), t[j]+(h/3.0));
        double k4 = h*(ff)(x+k3, t[j]+h);
        double newx = x + (k1+ 3.0*k2 + 3.0*k3 + k4)/8.0;

        /* we will try to print the peaks/valleys
         * by detecting if the new values are higher
         * or lower than the previous values */
        if (((x > lastx) && (newx < x)) 
        || ((x < lastx) && (newx > x))) 
            printer(x,t[j]);

        lastx = x;
        x = newx;
    }
    printer(x,tmax);
    return x;
}

// MAIN FUNCTION
int main() {
    std::vector<double> h_ = {
        0.1,
        0.025,
        0.0125,
        0.00625,
        0.003125,
        0.0015625
    };
    // this is a foreach loop to iterate over
    // the vector of h values. 
    for (const double h : h_ ) {
        char header[32];
        sprintf(header,"h = %.20f",h);
        pprint_head(header);

        double approx = rungekutta( fx, h, 1.0, 0.0, 2.0); 
        char appxs[32];
        sprintf(appxs,"approx = %.20f",approx);
        pprint_head(appxs);
        printf("\n");
    }
    return 0;
}

//table header printing function
void pprint_head(const string& head) {
    /* this is a bunch of horseshit for pretty-printing a table
     * _____________________________________
     * |              h = 302.394          |
     * |-----------------------------------|
     * | t= 0.022222222232 | x = 230940284 |
     * | t= 0.022222222232 | x = 230940284 |
     */
    using std::cout;
    using std::left;
    using std::setw;
    using std::setfill;
    int width = 60;
    int rwidth = 0;
    int lwidth = 0;
    if ((width-head.length()) % 2) {
        rwidth = (width-head.length())/2;
        lwidth = (width-head.length())/2 - 1;
    }
    else {
        rwidth = (width-head.length())/2 -1;
        lwidth = (width-head.length())/2 -1;
    }

    printf("%s\n", string(width,'-').c_str());
    printf("|%*c%s%*c|\n", lwidth, ' ', head.c_str(), rwidth, ' ');
    printf("%s\n", string(width,'-').c_str());
}
// compile:  g++ -std=c++11 -o hw4 hw4.cpp
// execute:  ./hw4
