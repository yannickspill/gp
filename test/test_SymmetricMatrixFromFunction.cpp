#include "SymmetricMatrixFromFunction.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>

namespace{
typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
}

class ConstCov {
   public:
    double eval(Eigen::Matrix<double, 1, 1> x,
                Eigen::Matrix<double, 1, 1> y) const {
        return 1.;
    }
};

int main(int, char * []) {
    ConstMat X(Eigen::MatrixXd::Random(10, 3));
    ConstCov cc;
    SymmetricMatrixFromFunction<ConstMat, ConstCov> cov(X, cc);
    Eigen::MatrixXd observed(cov.get());
    Eigen::MatrixXd expected(Eigen::MatrixXd::Ones(10, 10));
    for (unsigned i = 0; i < expected.rows(); i++)
        for (unsigned j = i; j < expected.cols(); j++)
            if (observed(i, j) != expected(i, j)) return 1;
    return 0;
}

