#include "SymmetricMatrixFromFunction.h"
#include "SolveDecomposedMatrix.h"
#include "Covariance1DFunction.h"
#include "ConstEigenObject.h"
#include "Scalar.h"
#include "LDLT.h"

#include <Eigen/Dense>
#include <math.h>

typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
typedef SymmetricMatrixFromFunction<ConstMat, Covariance1DFunction>
    VarCov;
typedef LDLT<VarCov> DecompVar;

int main(int, char * []) {
    //Build covariance matrix
    Scalar tau(1.);
    Scalar lambda(1.);
    Covariance1DFunction cov(tau,lambda);
    ConstMat X(Eigen::VectorXd::LinSpaced(10,0,1));
    VarCov Sigma(X, cov);
    //test DoubleInputVersionTracker
    if (!Sigma.get_is_function_of(tau)) return 1;
    if (!Sigma.get_is_function_of(lambda)) return 2;
    Scalar tau2(tau);
    if (!Sigma.get_is_function_of(tau2)) return 3;
    Scalar dummy(1.0);
    if (Sigma.get_is_function_of(dummy)) return 4;
    //Build LDLT
    DecompVar ldlt(Sigma);
    //test SingleInputCachedVersionTracker
    if (!ldlt.get_is_function_of(tau)) return 5;
    if (!ldlt.get_is_function_of(lambda)) return 6;
    if (!ldlt.get_is_function_of(tau2)) return 7;
    if (ldlt.get_is_function_of(dummy)) return 8;
    //Build Peps
    ConstMat eps(Eigen::VectorXd::Ones(10));
    SolveDecomposedMatrix<DecompVar, ConstMat> solve(ldlt, eps);
    //test DoubleInputCachedVersionTracker
    if (!solve.get_is_function_of(tau)) return 9;
    if (!solve.get_is_function_of(lambda)) return 10;
    if (!solve.get_is_function_of(tau2)) return 11;
    if (solve.get_is_function_of(dummy)) return 12;
    return 0;
}

