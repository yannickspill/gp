#include "VectorFromFunction.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>
#include <iostream>

namespace {
typedef ConstEigenObject<Eigen::VectorXd> ConstVec;
}

class ConstFunc {
  public:
    double eval(Eigen::Matrix<double, 1, 1> x) const {
        return 1.;
    }
    unsigned update() {
        return 0 ;
    }
};

int main(int, char * []) {
    ConstVec X(Eigen::VectorXd::Random(10));
    ConstFunc cc;
    VectorFromFunction<ConstVec, ConstFunc> cov(X, cc);
    Eigen::VectorXd observed(cov.get());
    Eigen::VectorXd expected(Eigen::VectorXd::Ones(10));
    if (observed != expected) return 1;
    return 0;
}

