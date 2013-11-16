#include "PriorCovarianceMatrix.h"
#include <Eigen/Dense>

class ConstantCov {
   public:
    double operator()(Eigen::Matrix<double, 1, 1> x,
                      Eigen::Matrix<double, 1, 1> y) const {
        return 1.;
    }
};

int main(int, char * []) {
    Eigen::MatrixXd X(Eigen::MatrixXd::Random(10, 3));
    ConstantCov cc;
    PriorCovarianceMatrix<ConstantCov> cov(X, cc);
    Eigen::MatrixXd observed(cov());
    Eigen::MatrixXd expected(Eigen::MatrixXd::Ones(10,10));
    if (observed != expected) return 1;
    return 0;
}

