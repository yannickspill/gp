#include "ConstEigenObject.h"
#include "MatrixSum.h"
#include "MatrixProduct.h"
#include "SymmetricMatrixFromFunction.h"
#include "Scalar.h"

#include <Eigen/Dense>

class ConstCovFunc {
  public:
    double eval(Eigen::RowVectorXd a, Eigen::RowVectorXd b) const {
        return 1.;
    }
    unsigned update() {
        return 0;
    }
};

int main(int, char * []) {
    //build sigma^2 * S
    typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
    ConstMat S(Eigen::MatrixXd::Identity(10, 10));
    Scalar sigma(0.0);
    typedef MatrixProduct<Scalar, ConstMat> MP;
    MP siS(sigma, S);
    typedef MatrixProduct<Scalar, MP> MPP;
    MPP si2S(sigma, siS);
    //build W
    ConstCovFunc cc;
    ConstMat X(Eigen::MatrixXd::Random(10, 10));
    typedef SymmetricMatrixFromFunction<ConstMat, ConstCovFunc> MF;
    MF W(X, cc);
    //sum the two
    typedef MatrixSum<MF, MPP> MS;
    MS cov(W, si2S);
    //test
    Eigen::MatrixXd observed(cov.get());
    Eigen::MatrixXd expected(Eigen::MatrixXd::Ones(10,10));
    if (observed != expected) return 1;
    //change sigma and test again
    sigma.set(3.0);
    Eigen::MatrixXd observed2(cov.get());
    expected += 9.0*Eigen::MatrixXd::Identity(10,10);
    if (observed2 != expected) return 2;
    return 0;
}

