#include "ConstEigenObject.h"
#include "VectorFromFunction.h"
#include "Linear1DFunction.h"
#include "SymmetricMatrixFromFunction.h"
#include "Covariance1DFunction.h"
#include "Scalar.h"
#include "MVN.h"

#include <Eigen/Dense>
#include <math.h>
#include <iostream>

namespace {
typedef ConstEigenObject<Eigen::VectorXd> ConstVec;
}

int main(int, char * []) {
    //build y
    ConstVec x(Eigen::VectorXd::LinSpaced(5,0,1));
    Scalar a(1.0);
    Scalar b(1.0);
    Linear1DFunction lf(a,b);
    typedef VectorFromFunction<ConstVec, Linear1DFunction> vff;
    vff y(x, lf);
    //build mu
    Scalar c(2.0);
    Scalar d(1.0);
    Linear1DFunction lfs(c,d);
    vff mu(x, lfs);
    //build Sigma
    Scalar lambda(1.0);
    Scalar tau(1.0);
    Covariance1DFunction cov(tau,lambda);
    typedef SymmetricMatrixFromFunction<ConstVec, Covariance1DFunction> smff;
    smff Sigma(x, cov);
    //build MVN
    MVN<vff, vff, smff> mvn(y,mu,Sigma);
    //
    //Test scalar dependency
    Scalar dummy(1.0);
    if (mvn.get_is_function_of(dummy)) return 1;
    if (!mvn.get_is_function_of(a)) return 2;
    if (!mvn.get_is_function_of(b)) return 3;
    if (!mvn.get_is_function_of(c)) return 4;
    if (!mvn.get_is_function_of(d)) return 5;
    if (!mvn.get_is_function_of(tau)) return 6;
    if (!mvn.get_is_function_of(lambda)) return 7;
    // test derivative wrt epsilon
    double db_obs(mvn.get_derivative(b));
    double db_exp(mvn.get_derivative_X().array().sum());
    if (std::abs(db_obs-db_exp) > 1e-7) return 8;
    double dd_obs(mvn.get_derivative(d));
    if (std::abs(dd_obs+db_obs) > 1e-7) return 9;
    //test derivative wrt sigma
    double dtau_obs(mvn.get_derivative(tau));
    Eigen::MatrixXd dsigma(mvn.get_derivative_Sigma());
    Eigen::MatrixXd dst(Sigma.get() * 2. / tau.get());
    double dtau_exp((dsigma*dst).trace());
    std::cout <<"dtau obs " << dtau_obs
              <<" dtau exp " << dtau_exp
              <<std::endl;
    if (std::abs(dtau_obs-dtau_exp) > 1e-7) return 10;
    return 0;
}

