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
 * g. PURPOSE:
 *
 * h. INPUT:
 *
 * i. PRECONDITIONS:
 *
 * j. OUTPUT:
 *    this program approximates an integral by the runge-kutta method.
 *    it also displays a few T and X values for each pass with differing
 *    H values.
 *
 * k. POSTCONDITIONS:
 *
 * l. ALGORITHM:
 *      for each step of t from 0 to 1
 *          do secant on f(t,x)
 *              to find
 *              where x(0) + x(1) - 3.0 = 0
 *      endfor
 *      do rungekutta on f(t,x)
 *          where x = x from secant method.
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

// this is important when using function pointers;{{{
// otherwise the syntax becomes even stupider
// and more cumbersome.}}}
typedef double (*func_dd)(double, double);
typedef double (*func_d)(double);


double rungekutta(func_dd func,
                  double h,
                  double xinit,
                  const double tmin,
                  const double tmax,
                  bool print);
// forward declare printer func
void pprint_head(const string& head);

// the problem to be solved
double fx(double x, double t) { 
    return x + (0.09 * std::pow(x, 2.0)) + std::cos(10.0 * t); 
}
// secant method, input 2 guesses
double secant(func_d func, double t0, double t1) {
    double b0 = func(t0);
    double b1 = func(t1);
    double e0 = t0+b0-3;
    double e1 = t1+b1-3;
    printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t0, b0, e0);
    printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t1, b1, e0);
    while (std::abs(e1) > 1E-4) {
        b0 = func(t0);
        b1 = func(t1);
        e0 = t1+b0-3;
        e1 = t1+b1-3;
        double tmp = t1;
        t1 = t1 - (t0-t1)/(e0-e1)*e1;
        t0 = tmp;
        printf("sec: x(0): %f  x(1):%f  x(0)+x(1)-3: %f\n", t1, b1,
                e1);
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
        auto printer = [&print](double x, double tt) {
            if (print)
            // lambda for printing
                printf("*   t=%.20f\tx=%.20f   *\n", tt, x);
        };
    // the initial value is the first x value
    double x = xinit;
    //double lastx = xinit;

    // create a t vector of correct size
    // example: let t = { 0.0 , 2 }
    //     h = 0.1,    size = 20
    //     h = 0.025,  size = 40
    //     h = 0.0125, size = 80
    //     etc.
    //std::vector<double> t(int((tmax - tmin) / h));
    //int print_every_nth = std::max((int)((tmax-tmin)/h), 10);
    int print_every_nth = 10;
    int j = 0;
    for (double t = 0.0; t < tmax;t = t + h) {
        // fill the t vector
        //t[j] = tmin + j * h;

        /* classical runge-kutta uses
         * k2 = hf(x+k1/2, t+h/2)
         * and k3 = hf(x+k2/2, t+h/2)
         *
         * But we have to try a variant so we use
         * k2 = hf(x + k1/3, t+h/3)
         * k3 = hf(x + k1/3, t+h/3)
         * and make x(t + h) = (k1 + 3k2 + 3k3 + k4)/8
         */
        double k1 = h*func(x, t);
        double k2 = h*func(x + (k1 / 3.0), t + h/3.0);
        double k3 = h*func(x + (k2 / 3.0), t + h/3.0);
        double k4 = h*func(x + k3, t + h);
        x += (1/8.0) * (k1 + 3.0 * k2 + 3.0 * k3 + k4);

        if (!(j%print_every_nth))
            printer(x, t);
        j++;
    }
    printer(x, tmax);
    return x;
}
/*}}}*/

/*{{{
double sec(double x0, double x1) {
    // [t,y] = ode45(odefun,tspan,y0)
    do {
        double tmp = x1;
        double y1 = rungekutta(fx,0.025,x0,0.0,1.0);
        double y2 = rungekutta(fx,0.025,x1,0.0,1.0);
        double m1 = y1 - 3;
        double m2 = y2 - 3;
        double x2 = x0 - (m1*(x0-x1)/(m1-m2));
        x1 = x2;
        x0 = tmp;
        printf("y1: %f    y2: %f\n", y1,y2);
    } while (abs(x1-x0) > 1E-4);
    return x1;
}*//*}}}*/

// MAIN FUNCTION
int main() {
    int tmax = 1;
    int tmin = 0;
    double h = 0.025;
    /* this lambda decomposes the rk4
     * into a function with only 1 parameter so
     * it can be passed to secant method. */
    auto rk4_compo = [](double t) {
        return rungekutta(fx, 0.025, t, 0.0, 1.0, false);
    };

    double sec_approx = secant(rk4_compo, 0.7, 1.0);

    rungekutta(fx, 0.025, sec_approx, 0.0, 1.0, true);

        /*double b = rk4_compo(0.7);
        printf("b = %f\n", b);
        double b2 = rk4_compo(1.0);
    for (int j=0; j<5; j++) {
        double m1 = 3-b;
        double m2 = 3-b2;
        printf("b = %f\n", b2);
        b2 = b2 - ((m2)*(b-b2))/(m1-m2);
        printf("b = %f\n", b2);
        */
    //}


    //double x1 = testsecant(rk4_compo, 0.7,1.0);
    //double x2 = sec(rk4_compo, 0.7, 1.0);
    //double rk1 = rk4_compo(0.7);
    //printf("x(1): %f\n" , rk1);
    //printf("x(0): %f\n", secant(fx, 


    //secant(rk4_compo, 0.7, 1.0);
  /* 
    double a01 = 0.7;
    double a02 = 1.0;
    double a11 = rk4_compo(a01);
    double a12 = rk4_compo(a02);
    double a03 = 3-a11;
    double a04 = 3-a12;
    double a13 = rk4_compo(a03);
    double a14 = rk4_compo(a04);
    double a05 = 3-a13;
    double b5 = rk4_compo(a05);
    printf("x(0):%f   x(1):%f\n", a01,a11);
    printf("x(0):%f   x(1):%f\n", a02,a12);
    printf("x(0):%f   x(1):%f\n", a03,a11);
    printf("x(0):%f   x(1):%f\n", a04,a12);
    printf("x(0):%f   x(1):%f\n", a03,a13);
    printf("x(0):%f   x(1):%f\n", a05,b5);
    int i = 0;
    double a = 0.7;
    double a2= 1.0;
    double b;
    do {
    //for (int i=0; ;i++) {
        b = secant(rk4_compo, a, a2);
        xapprox = secant(rk4_compo, nextt[i], nextt[i+1]);
        nextt.push_back(xapprox);
        printf("x(0):%.12f   x(1):%.12f  x(0)+x(1)-3= %f\n",
                nextt[i+1],3-xapprox, xapprox-nextt.back());
        double x1 = testfunc(rk4_compo, x0);
        printf("x(0) = %f    x(1) = %f\n", x0, x1);
        
        //x0 = x1;
        
        i++;
    } while (abs(xapprox-nextt.back()) > 1E-4);
    printf("x(0):%.12f   x(1):%.12f  x(0)+x(1)-3=%f\n",nextt[i+1],3-xapprox, xapprox-nextt.back());
    */
   

    /*
    std::vector<double> t(int((tmax - tmin) /h)+1);
    // fill vector for each step of t+h, from 0 to 2
    // (40 steps for this example);
    for (int j = 0; j < t.size(); j++) {
        t[j] = (j ) * h;
        auto sec_iter = [&](double x) {
            fx(x, t[j]);
        };
        double x2 = secant(fx,t[j],0.7,1.0);
        printf("t[%d] = %f\n",j,t[j]);
    }*/
    //printf("test: %f\n", rungekutta(rktest, 0.00125625, 1.0, 0.0, 2.0));

    return 0;
    /*{{{
    std::vector<double> h_ = { 0.1,     0.025,    0.0125,
                               0.00625, 0.003125, 0.0015625 };
    // this is a foreach loop to iterate over
    // the vector of h values.
    for (const double h : h_) {
        char header[32];
        sprintf(header, "h = %.20f", h);
        pprint_head(header);

        double approx = rungekutta(fx, h, 1.0, 0.0, 2.0);
        char appxs[32];
        sprintf(appxs, "approx = %.20f", approx);
        pprint_head(appxs);
        printf("\n");
    }
    return 0;
    }}}*/
}

// table header printing function
void pprint_head(const string& head) {
    /* {{{
     * this is a bunch of horseshit for pretty-printing a table
     * _____________________________________
     * |              h = 302.394          |
     * |-----------------------------------|
     * | t= 0.022222222232 | x = 230940284 |
     * | t= 0.022222222232 | x = 230940284 |
     */
    int width = 60;
    int rwidth = 0;
    int lwidth = 0;
    if ((width - head.length()) % 2) {
        rwidth = (width - head.length()) / 2;
        lwidth = (width - head.length()) / 2 - 1;
    }
    else {
        rwidth = (width - head.length()) / 2 - 1;
        lwidth = (width - head.length()) / 2 - 1;
    }

    printf("%s\n", string(width, '*').c_str());
    printf("*%*c%s%*c*\n", lwidth, ' ', head.c_str(), rwidth, ' ');
    printf("%s\n", string(width, '*').c_str());
    /*}}} */
}
// compile:  g++ -std=c++11 -o hw5 hw5.cpp
// execute:  ./hw5
