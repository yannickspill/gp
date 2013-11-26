#include "benchmark_helpers.h"
#include "macros.h"
#include "MVN.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>

typedef Eigen::VectorXd EigenVec;
typedef Eigen::MatrixXd EigenMat;
typedef ConstEigenObject<EigenVec> Vec;
typedef ConstEigenObject<EigenMat> Mat;
typedef MVN<Vec, Vec, Mat> Multivariate;

// Just execute one get(), get_derivative_FM() and get_derivative_Sigma()
// for a given dimension. Purpose is profiling and checking if cache works
int main(int, char * []) {
    // dimension which will be tried
    const unsigned n = 1000;
    // Build inputs
    Vec y(EigenVec::Constant(n, 1.0));
    Vec m(EigenVec::LinSpaced(n, 0, 1));
    EigenVec x(EigenVec::LinSpaced(n, 0, 1));
    std::vector<double> params(NUM_PARAMS, 0.);
    params[PARAM_TAU] = 1.;
    params[PARAM_LAMBDA] = 0.1;
    Mat Sigma(get_prior_covariance(x, params, 0.01));
    // perform run
    unsigned ncalls = 0;
    std::cout << "mvn construct" << std::endl;
    Multivariate mvn(y, m, 0.0, Sigma);
    std::cout << "mvn get" << std::endl;
    mvn.get();
    std::cout << "mvn dFM" << std::endl;
    mvn.get_derivative_FM();
    std::cout << "mvn dSigma" << std::endl;
    mvn.get_derivative_Sigma();
    std::cout << "mvn done" << std::endl;
}
