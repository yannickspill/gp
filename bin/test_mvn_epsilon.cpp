#include "mvn_epsilon.h"

#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;

class ConstantMean {
   public:
    double operator()(RowVectorXd x_) const { return 1.; }
};

int main(int, char * []) {
    MatrixXd X(MatrixXd::Random(10, 3));
    VectorXd y(VectorXd::LinSpaced(10, 0, 9));
    ConstantMean cm;
    EpsilonVector<ConstantMean> ev(X, y, cm);
    VectorXd observed(ev());
    VectorXd expected(y - VectorXd::Ones(10));
    if (observed != expected) return 1;
    return 0;
}

