#include "SymmetricMatrixFromFunction.h"
#include "Covariance1DFunction.h"
#include "ConstEigenObject.h"
#include "Scalar.h"
#include "LDLT.h"

#include <Eigen/Dense>
#include <math.h>

typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
typedef SymmetricMatrixFromFunction<ConstMat, Covariance1DFunction>
    VarCov;

int main(int, char * []) {
    //check if result is correct
    Eigen::MatrixXd m(2, 2);
    m << 1, .5, .5, 1;
    ConstMat cc(m);
    LDLT<ConstMat> ldlt(cc);
    if (ldlt.get_log_determinant() != std::log(0.75)) return 1;
    //check if caching works
    Scalar tau(1.);
    Scalar lambda(1.);
    Covariance1DFunction cov(tau,lambda);
    ConstMat X(Eigen::VectorXd::LinSpaced(10,0,1));
    VarCov Sigma(X, cov);
    LDLT<VarCov> ldlt2(Sigma);
    double det1 = ldlt2.get_log_determinant();
    tau.set(10);
    ldlt2.update();
    double det2 = ldlt2.get_log_determinant();
    if (std::abs(det1 - det2)<1) return 2;
    return 0;
}

