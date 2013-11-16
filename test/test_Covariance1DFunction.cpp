#include "Scalar.h"
#include "Covariance1DFunction.h"

#include <iostream>
#include <math.h>

int main(int, char * []) {
    Scalar tau(1.0);
    Scalar lambda(3.0);
    Covariance1DFunction f(tau, lambda);
    if (f(1,2) != std::exp(-0.5*(1/9.))) return 1;
    tau.set(2.);
    if (f(4,2) != 4*std::exp(-0.5*(4/9.))) return 2;
    return 0;
}

