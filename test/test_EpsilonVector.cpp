#include "EpsilonVector.h"
#include "Linear1DFunction.h"
#include "VectorFromFunction.h"
#include "macros.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>

namespace {
typedef ConstEigenObject<Eigen::VectorXd> ConstVec;
typedef VectorFromFunction<ConstVec, Linear1DFunction> VFF;
typedef EpsilonVector<ConstVec, ConstVec> MD;
typedef EpsilonVector<ConstVec, VFF> MD2;
typedef EpsilonVector<VFF, ConstVec> MD3;
}
int main(int, char*[]) {
    Eigen::VectorXd A(Eigen::VectorXd::Random(10));
    Eigen::VectorXd B(Eigen::VectorXd::Random(10));
    const ConstVec cA(A), cB(B);
    MD diff(cA, cB);
    //test value
    if ((diff.get()-(A-B)).array().abs().array().sum() > 1e-7) return 1;
    const Eigen::VectorXd ones(Eigen::VectorXd::Ones(10));
    //test left derivative
    if ((diff.get_derivative_in1() - ones).array().abs().array().sum() > 1e-7)
        return 2;
    //test right derivative
    if ((diff.get_derivative_in2() + ones).array().abs().array().sum() > 1e-7)
        return 3;
    //test derivative wrt scalar on the right
    Scalar a(0.0);
    Scalar b(1.0);
    const ConstVec X(Eigen::VectorXd::LinSpaced(10, 0, 9));
    Linear1DFunction f(a, b);
    VFF mean(X, f);
    ConstVec y(Eigen::VectorXd::LinSpaced(10, 0, 9));
    MD2 eps2(y, mean);
    if ((eps2.get_derivative(b) + ones).array().abs().array().sum() > 1e-7)
        return 5;
    //right
    MD3 eps3(mean, y);
    if ((eps3.get_derivative(b) - ones).array().abs().array().sum() > 1e-7)
        return 6;
    return 0;
}
