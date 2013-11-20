#include "MVN.h"
#include "ConstEigenObject.h"
#include "macros.h"

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

#include <Eigen/Dense>

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define NUM_PARAMS 4
#define PARAM_A 0
#define PARAM_SIGMA2 1
#define PARAM_TAU 2
#define PARAM_LAMBDA 3

#define NUM_DATA 3
#define DATA_X 0
#define DATA_Y 1
#define DATA_YERR 2

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXi;

namespace {
typedef ConstEigenObject<VectorXd> ConstVec;
typedef ConstEigenObject<MatrixXd> ConstMat;
typedef MVN<ConstVec, ConstVec, ConstMat> Multivariate;
}

void read_input_parameters(char* fname,
                           std::vector<std::vector<double> >& params,
                           std::vector<bool>& use_deriv) {
    std::string line;

    std::ifstream fl(fname);
    if (!fl.is_open()) abort();

    while (getline(fl, line)) {
        // skip comments
        if (line.substr(0, 1) == "#") continue;

        // scan next line for params
        std::vector<double> param;
        std::istringstream iss(line);
        for (unsigned i = 0; i < NUM_PARAMS; i++) {
            std::string sub;
            iss >> sub;
            param.push_back(std::atof(sub.c_str()));
        }
        params.push_back(param);

        // add flag
        std::string sub;
        iss >> sub;
        use_deriv.push_back((bool)std::atoi(sub.c_str()));
    }
    return;
}

void read_input_data(char* fname, std::vector<std::vector<double> >& data) {
    std::string line;

    std::ifstream fl(fname);
    if (!fl.is_open()) abort();

    while (getline(fl, line)) {
        // skip comments
        if (line.substr(0, 1) == "#") continue;

        // scan next line for params
        std::vector<double> tokens;
        std::istringstream iss(line);
        for (unsigned i = 0; i < NUM_DATA; i++) {
            std::string sub;
            iss >> sub;
            tokens.push_back(std::atof(sub.c_str()));
        }
        data.push_back(tokens);
    }
    return;
}

ConstVec get_vector_column(const std::vector<std::vector<double> >& data,
                           const unsigned colno) {
    VectorXd retval(data.size());
    for (unsigned i = 0; i < data.size(); i++) retval(i) = data[i][colno];
    return ConstVec(retval);
}

ConstVec get_mean_vector(ConstVec x_values, const std::vector<double> params) {
    double A = params[0];
    return ConstVec(VectorXd::Constant(x_values.get().rows(), A));
}

inline double get_covariance_function(double x1, double x2, double lambda,
                                      double tausq) {
    return tausq * std::exp(-0.5 * SQUARE((x1 - x2) / lambda));
}

MatrixXd get_prior_covariance(VectorXd x_values,
                              const std::vector<double> params) {
    unsigned M = x_values.rows();
    MatrixXd retval(M, M);
    double tausq = SQUARE(params[PARAM_TAU]);
    double lambda = params[PARAM_LAMBDA];
    // only fill upper triangle
    for (unsigned i = 0; i < M; i++)
        for (unsigned j = i; j < M; j++)
            retval(i, j) = get_covariance_function(x_values(i), x_values(j),
                                                   lambda, tausq);
    return MatrixXd(retval.selfadjointView<Eigen::Upper>());
}

ConstMat get_covariance_matrix(ConstVec x_values, ConstVec S,
                               const std::vector<double> params) {
    double sigma2 = params[PARAM_SIGMA2];
    return ConstMat(sigma2 * MatrixXd(S.get().asDiagonal()) +
                    get_prior_covariance(x_values.get(), params));
}

bool mean_has_changed(const std::vector<std::vector<double> > params,
                      unsigned i) {
    if (i == 0) return false;
    bool chA = std::abs(params[i][PARAM_A] - params[i - 1][PARAM_A]) > 1e-5;
    return chA;
}

bool covariance_has_changed(const std::vector<std::vector<double> > params,
                            unsigned i) {
    if (i == 0) return false;
    bool chsi =
        std::abs(params[i][PARAM_SIGMA2] - params[i - 1][PARAM_SIGMA2]) > 1e-5;
    bool chla =
        std::abs(params[i][PARAM_LAMBDA] - params[i - 1][PARAM_LAMBDA]) > 1e-7;
    bool chtau =
        std::abs(params[i][PARAM_TAU] - params[i - 1][PARAM_TAU]) > 1e-5;
    return chsi || chla || chtau;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << argv[0] << " profile.dat schedule.dat" << std::endl;
        std::cout << "profile.dat must have 3 columns: x, y, yerr" << std::endl;
        std::cout << "schedule.dat must have 5 columns: A, sigma2, tau, lambda"
                  << std::endl
                  << "and whether the derivative is to be calculated"
                  << std::endl;
        return 1;
    }

    // read input data
    std::vector<std::vector<double> > data;
    read_input_data(argv[1], data);
    // process data to Eigen form
    ConstVec x_values(get_vector_column(data, 0));
    ConstVec y_values(get_vector_column(data, 1));
    ConstVec S(get_vector_column(data, 2).get().array().square());

    // read input params
    std::vector<std::vector<double> > params;
    std::vector<bool> use_deriv;
    read_input_parameters(argv[2], params, use_deriv);
    // process data to Eigen form
    ConstVec mu(get_mean_vector(x_values, params[0]));
    ConstMat Sigma(get_covariance_matrix(x_values, S, params[0]));

    // create MVN object
    const double lJA = 0.0;  // log jacobian is 0 for normal distribution
    Multivariate mvn(y_values, mu, lJA, Sigma);

    // loop over parameter lists
    std::cout << "START " << params.size() << " steps for dataset of size "
              << data.size() << std::endl;
    for (unsigned i = 0; i < params.size(); i++) {
        std::vector<double> param(params[i]);
        // update matrices if needed
        if (mean_has_changed(params, i))
            mvn.set_FM(get_mean_vector(x_values, param));
        if (covariance_has_changed(params, i))
            mvn.set_Sigma(get_covariance_matrix(x_values, S, param));
        // compute derivatives if requested
        if (use_deriv[i]) {
            mvn.get_derivative_FM();
            mvn.get_derivative_Sigma();
        }
        mvn.get();
    }
    std::cout << "DONE" << std::endl;
    return 0;
}
