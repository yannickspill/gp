#include "SolveDecomposedMatrix.h"
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
typedef LDLT<ConstMat> DecompConst;
typedef LDLT<VarCov> DecompVar;

int main(int, char * []) {
    //check if result is correct
    Eigen::MatrixXd m(2, 2);
    m << 1, .5, .5, 1;
    ConstMat cc(m);
    DecompConst ldlt(cc);
    SolveDecomposedMatrix<DecompConst, ConstMat> solve(ldlt, cc);
    if (solve.get() != Eigen::MatrixXd::Identity(2,2)) return 1;
    //check if caching works
    Scalar tau(1.);
    Scalar lambda(.01);
    Covariance1DFunction cov(tau,lambda);
    ConstMat X(Eigen::VectorXd::LinSpaced(10,0,1));
    VarCov Sigma(X, cov);
    ConstMat Rhs(Eigen::VectorXd(Sigma.get().row(0)));
    DecompVar ldlt2(Sigma);
    SolveDecomposedMatrix<DecompVar, ConstMat> solve2(ldlt2, Rhs);
    Eigen::VectorXd Lhs(10);
    Lhs.setZero();
    Lhs(0)=1;
    Eigen::VectorXd obs(solve2.get());
    if (obs != Lhs) return 2;
    tau.set(2);
    Lhs(0)=1/4.;
    solve2.update();
    Eigen::VectorXd obs2(solve2.get());
    if (obs2 != Lhs) return 3;
    return 0;
}

