#include "Scalar.h"
#include "Linear1DFunction.h"
#include <iostream>

int main(int, char * []) {
    Scalar a(1.0);
    Scalar b(3.0);
    Linear1DFunction f(a, b);
    if (f.eval(1) != 4.) return 1;
    b.set(2.);
    if (f.eval(3) != 5.) return 2;
    a.set(-1);
    if (f.eval(1) != 1.) return 3;
    if (f.eval_derivative(2.,a) != 2.) return 4;
    if (f.eval_derivative(3.,b) != 1.) return 5;
    Scalar c(1.);
    if (f.eval_derivative(3.,c) != 0.) return 6;
    return 0;
}

