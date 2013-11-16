#include "PosteriorCovarianceMatrix.h"
#include "Scalar.h"

#include <Eigen/Dense>

class ConstantCov {
   public:
       Eigen::MatrixXd operator()() const {
        return Eigen::MatrixXd::Ones(10,10);
    }
};

int main(int, char * []) {
    Eigen::MatrixXd S(Eigen::MatrixXd::Identity(10, 10));
    Scalar sigma(0.0);
    ConstantCov cc;
    PosteriorCovarianceMatrix<ConstantCov> cov(S, sigma, cc);
    Eigen::MatrixXd observed(cov());
    Eigen::MatrixXd expected(Eigen::MatrixXd::Ones(10,10));
    if (observed != expected) return 1;
    sigma.set(3.0);
    observed = cov();
    expected += 9.0*Eigen::MatrixXd::Identity(10,10);
    if (observed != expected) return 2;
    return 0;
}

