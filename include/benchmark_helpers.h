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

VectorXd get_vector_column(const std::vector<std::vector<double> >& data,
                           const unsigned colno) {
    VectorXd retval(data.size());
    for (unsigned i = 0; i < data.size(); i++) retval(i) = data[i][colno];
    return retval;
}

VectorXd get_mean_vector(VectorXd x_values, const std::vector<double> params) {
    double A = params[0];
    return VectorXd::Constant(x_values.rows(), A);
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

MatrixXd get_covariance_matrix(VectorXd x_values, VectorXd S,
                               const std::vector<double> params) {
    double sigma2 = params[PARAM_SIGMA2];
    return sigma2 * MatrixXd(S.asDiagonal()) +
           get_prior_covariance(x_values, params);
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

