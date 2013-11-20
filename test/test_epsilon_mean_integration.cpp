#include "Scalar.h"
#include "ConstEigenObject.h"
#include "Linear1DFunction.h"
#include "VectorFromFunction.h"
#include "MatrixDifference.h"

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;
namespace{
typedef ConstEigenObject<VectorXd> ConstVec;
typedef VectorFromFunction<ConstVec, Linear1DFunction> VFF;
typedef MatrixDifference<ConstVec, VFF> MD;
}


int main(int, char * []) {
    //create a vector from a Linear1DFunction
    ConstVec X(VectorXd::LinSpaced(10, 0, 9));
    Scalar a(1.0);
    Scalar b(2.0);
    Linear1DFunction f(a, b);
    VFF mean(X, f);
    //create second vector as constant
    ConstVec y(VectorXd::LinSpaced(10, 0, 9));
    //create difference object
    MD eps(y, mean);
    //
    VectorXd observed(eps.get());
    VectorXd expected(10);
    expected.setConstant(-2.);
    if (observed != expected) return 1;
    //
    b.set(1.0);
    VectorXd observed2 = eps.get();
    expected.setConstant(-1.);
    if (observed2 != expected) return 2;
    return 0;
}
