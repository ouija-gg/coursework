/*
     a. PROGRAMMER: Timothy Vicars 

     b. COURSE:  CSCI 3321 Numerical Methods 

     c. DATE:   March 24, 2019

     d. ASSIGNMENT:  Number 3

     e. ENVIRONMENT: Arch Linux, Clang/GCC C++

     f. FILES INCLUDED: hw3.cpp

     g. PURPOSE:  
        This program calculates integrals of 3 different curves, using
        3 different approximation methods; including Newton-Cotes,
        Guassian quadrature, and Lobatto.

     h. INPUT: 

     i. PRECONDITIONS:  

     j. OUTPUT:  
        This programs outputs the apprixmate integral with 20 digits
        and the error from the exact answer.

     k. POSTCONDITIONS: 

     l. ALGORITHM: 
        Lobatto and Guassian: I = w_i * f(x_i)  
        Newton : I = (2h/45) (w_i * f(x_i))

     m.  ERRORS:  

     n.  EXAMPLE: ./hw3

*/
#include <stdio.h>
#include <math.h>

/* we choose c++ because math operations
 * are overloaded for doubles and simpler
 * to read
 */

//this is important when using function pointers;
//otherwise the syntax becomes even stupider
//and more cumbersome.
typedef double (*func)(double);

//this is curve 1
double fx1(double x) {
    return 1 - sin(1-x);
}
double fx2(double x) {
    return 1 + sqrt(x+1);
}
double fx3(double x) {
    return tanh(x+1);
}
double newtoncotes(func fx, double a, double b) {
    //we use a function^pointer here so we can
    //use this method on different curves by passing
    //in a different fx function pointer;

    double h = (b-a)/4;
    double x[5];
    //load up our x values
    for (int i = 0; i <= 4; i++) {
        x[i] = a+(i*h);
    }
    //we can use boole's rule because we are using a
    //5-pt quad. which makes our weights 7,32,12,32,7
    const int w[5] = {7, 32, 12, 32, 7};

    //now we can finally compute the approx
    double sum = 0;
    for (int i = 0; i<=4; i++) {
        //this is ugly because we are using function
        //pointer, but basically it is the same as
        //w[i]*fx(i)
        sum += (w[i]*((fx)(x[i])));
    }
    return (double)((2*h)/45)*sum;
}

double guass(func fx) {
    //i am going to use the legendre variant
    //because the weights are simpler/static.
    const double w[5] = { 0.236927, 0.478629, 0, 0.478629, 0.236927 };
    //legendre also has approx for X_i 
    const double x[5] = { -0.90618, -0.538469, 0, 0.538469, 0.90618 };
    double sum = 0;
    for (int i = 0; i<= 4; i++)
        sum += (w[i])*((fx)(x[i]));
    return sum;
}

double lobatto(func fx) {
    //weights given by professor
    const double w[5] = {
        0.1, (49.0/90.0), 32.0/45.0 , 49.0/90.0 , 0.1
    };
    //x_i given by professor
    const double x[5] = {
        -1.0, -sqrt(3.0/7.0), 0, sqrt(3.0/7.0), 1 
    };
    double sum = 0;
    for (int i=0; i<=4;i++)
        sum += (w[i])*((fx)(x[i]));
    return sum;
}
 
int main() {
    printf("----Curve 1----\n");
    printf("  Newton-Cotes%5s %.20lf"," : ", newtoncotes(fx1,-1.0,1.0));
    printf("  error%5s %lf\n"," : ", abs(0.58385316345285761300-newtoncotes(fx1,-1.0,1.0)));
    printf("  Guassian%9s %.20lf"," : ", guass(fx1));
    printf("  error%5s %lf\n"," : ", abs(0.58385316345285761300-guass(fx1)));
    printf("  Lobatto%10s %.20lf"," : ", lobatto(fx1));
    printf("  error%5s %lf\n"," : ", abs(0.58385316345285761300-lobatto(fx1)));
    //Curve 2
    printf("\n----Curve 2----\n");
    printf("  Newton-Cotes%5s %.20lf"," : ", newtoncotes(fx2,-1.0,1.0));
    printf("  error%5s %lf\n"," : ", abs(3.8856180831641267317-newtoncotes(fx2,-1.0,1.0)));
    printf("  Guassian%9s %.20lf"," : ", guass(fx2));
    printf("  error%5s %lf\n"," : ", abs(3.8856180831641267317-guass(fx2)));
    printf("  Lobatto%10s %.20lf"," : ", lobatto(fx2));
    printf("  error%5s %lf\n"," : ", abs(3.8856180831641267317-lobatto(fx2)));
    //Curve 3
    printf("\n----Curve 3----\n");
    printf("  Newton-Cotes%5s %.20lf"," : ", newtoncotes(fx3,-1.0,1.0));
    printf("  error%5s %lf\n"," : ", abs(1.3250027473578644309-newtoncotes(fx3,-1.0,1.0)));
    printf("  Guassian%9s %.20lf"," : ", guass(fx3));
    printf("  error%5s %lf\n"," : ", abs(1.3250027473578644309-guass(fx3)));
    printf("  Lobatto%10s %.20lf"," : ", lobatto(fx3));
    printf("  error%5s %lf\n"," : ", abs(1.3250027473578644309-lobatto(fx3)));

    return 0;
}
// compile:  g++ -o hw3 hw3.cpp
// execute:  ./hw3
