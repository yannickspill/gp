#include "Scalar.h"
#include "Linear1DFunction.h"
#include <iostream>

int main(int, char * []) {
    Scalar a(1.0);
    Scalar b(3.0);
    Linear1DFunction f(&a, &b);
    if (f(1) != 4.) return 1;
    b.set(2.);
    if (f(3) != 5.) return 2;
    a.set(-1);
    if (f(1) != 1.) return 3;
    return 0;
}

