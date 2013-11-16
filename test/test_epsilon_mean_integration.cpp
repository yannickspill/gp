#include "mvn_EpsilonVector.h"
#include "Linear1DFunction.h"
#include "Scalar.h"

#include <Eigen/Dense>

int main(int, char * []) {
    Scalar a(1.0);
    Scalar b(2.0);
    Linear1DFunction f(a, b);
    MatrixXd X(VectorXd::LinSpaced(10, 0, 9));
    VectorXd y(VectorXd::LinSpaced(10, 0, 9));
    EpsilonVector<Linear1DFunction> eps(X, y, f);
    VectorXd observed(eps());
    VectorXd expected(10);
    expected.setConstant(-2.);
    if (observed != expected) return 1;
    b.set(1.0);
    observed = eps();
    expected.setConstant(-1.);
    if (observed != expected) return 2;
    return 0;
}
