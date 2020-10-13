/*
 * a. PROGRAMMER: Timothy Vicars
 * {{{
 * b. COURSE:  CSCI 3321 Numerical Methods
 *
 * c. DATE:   April 25, 2019
 *
 * d. ASSIGNMENT:  Number 5
 *
 * e. ENVIRONMENT: Arch Linux, Clang/GCC C++
 *
 * f. FILES INCLUDED: hw5.cpp
 *
 * g. PURPOSE: This program approximates an F(x,t) function's
 *      x(0), by using the secant method in conjuction with the
 *      rungekutta4 method.
 *
 * h. INPUT:
 *
 * i. PRECONDITIONS:
 *
 * j. OUTPUT:
 *      This program outputs x(0), x(1) and the boundary condition
 *      from the secant method. then outputs t,x values for the runge kutta
 * method. k. POSTCONDITIONS:
 *
 * l. ALGORITHM:
 *     while (error > 1E-4) then
 *          xn+1 = secant(xn)
 *     rk4(xn)
 *
 * m.  ERRORS:
 *
 * n.  EXAMPLE: ./hw5
 *
 * x.  NOTES:
 *     we choose c++ because math operations
 *     are overloaded for doubles and simpler
 *     to read. We could use C's tmath.h library
 *     as well.
 * }}}
 */
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdio.h>
#include <string>
#include <vector>

using std::string;

// {{{ this is important when using function pointers;
// otherwise the syntax becomes even stupider
// and more cumbersome.}}}
typedef double (*func_dd)(double, double);
typedef double (*func_d)(double);

// the problem to be solved
double fx(double x, double t) {
    return x + (0.09 * std::pow(x, 2.0)) + std::cos(10.0 * t);
}
// secant method, input 2 guesses
double secant(func_d func, double t0, double t1) {
    double b0 = func(t0);
    double b1 = func(t1);
    double e0 = t0 + b0 - 3; //bounds condition
    double e1 = t1 + b1 - 3;  //bounds condition
    printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t0, b0, e0);
    printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t1, b1, e0);
    while (std::abs(e1) > 1E-4) {
        b0 = func(t0);
        b1 = func(t1);
        e0 = t1 + b0 - 3;
        e1 = t1 + b1 - 3;
        double tmp = t1;
        t1 = t1 - (t0 - t1) / (e0 - e1) * e1;
        t0 = tmp;
        printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t1, b1, e1);
    }
    return t1;
}
// rungekutta method {{{
double rungekutta(func_dd func,
                  double h,
                  double xinit,
                  const double tmin,
                  const double tmax,
                  const bool print = 0) {

    // lambda for printing
    auto printer = [&print](double x, double tt) {
        if (print)
            printf("*   t=%.20f\tx=%.20f   *\n", tt, x);
    };
    // the initial value is the first x value
    double x = xinit;

    /* example: let t = { 0.0 , 2 }
         h = 0.1,    size = 20
         h = 0.025,  size = 40
         h = 0.0125, size = 80
         etc.
    */
    int print_every_nth = 10;
    int j = 0;
    for (double t = 0.0; t < tmax; t = t + h) {

        /* classical runge-kutta uses
         * k2 = hf(x+k1/2, t+h/2)
         * and k3 = hf(x+k2/2, t+h/2)
         *
         * But we have to try a variant so we use
         * k2 = hf(x + k1/3, t+h/3)
         * k3 = hf(x + k1/3, t+h/3)
         * and make x(t + h) = (k1 + 3k2 + 3k3 + k4)/8
         */
        double k1 = h * func(x, t);
        double k2 = h * func(x + (k1 / 3.0), t + h / 3.0);
        double k3 = h * func(x + (k2 / 3.0), t + h / 3.0);
        double k4 = h * func(x + k3, t + h);
        x += (1 / 8.0) * (k1 + 3.0 * k2 + 3.0 * k3 + k4);

        if (!(j % print_every_nth))
            printer(x, t);
        j++;
    }
    // always print the last t
    printer(x, tmax);
    return x;
}
/*}}}*/

// MAIN FUNCTION
int main() {
    /* this lambda decomposes the rk4
     * into a function with only 1 parameter so
     * it can be passed to secant method. */
    auto rk4_compo = [](double t) {
        return rungekutta(fx, 0.025, t, 0.0, 1.0, false);
    };
    /* get the x(0) from secant method using
     * the rk4 method for the function.*/
    double sec_approx = secant(rk4_compo, 0.7, 1.0);

    /* use the secant approximate x(0) as x(0) in rk4 */
    rungekutta(fx, 0.025, sec_approx, 0.0, 1.0, true);
    return 0;
}

// compile:  g++ -std=c++11 -o hw5 hw5.cpp
// execute:  ./hw5
