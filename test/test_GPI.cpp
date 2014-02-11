#include "Matrix.h"
#include "Scalar.h"
#include "GPI.h"
#include "MVN.h"
#include "mean_functions.h"
#include "covariance_functions.h"

#include <Eigen/Dense>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace {

unsigned get_int_token(const std::string& line) {
    std::istringstream iss(line);
    std::string sub;
    iss >> sub;
    return std::atoi(sub.c_str());
}

std::vector<double> get_float_tokens(const std::string& line, unsigned len) {
    // scan next line for params
    std::vector<double> tokens;
    std::istringstream iss(line);
    for (unsigned i = 0; i < len; i++) {
      std::string sub;
      iss >> sub;
      tokens.push_back(std::atof(sub.c_str()));
    }
    return tokens;
}

Eigen::VectorXd std_to_eigen_vector(const std::vector<double>& in) {
    Eigen::VectorXd out(in.size());
    for (size_t i=0; i<in.size(); ++i)
        out(i) = in[i];
    return out;
}

Eigen::MatrixXd read_input_data(char* fname, unsigned ncols) {
  std::string line;
  std::ifstream fl(fname);
  if (!fl.is_open()) abort();

  std::vector<std::vector<double> > data;
  while (getline(fl, line)) {
    // skip comments
    if (line.substr(0, 1) == "#") continue;
    data.push_back(get_float_tokens(line, ncols));
  }
  Eigen::MatrixXd ret(data.size(), data[0].size());
  return ret;
}

void read_input_params(char* fname, GP::VectorXd& q, GP::VectorXd& I, GP::VectorXd& err,
                       unsigned& N, unsigned& ndraws,
                       std::vector<double>& values) {
    std::string line;
    std::ifstream fl(fname);
    if (!fl.is_open()) abort();

    //skip first comment line
    getline(fl, line);
    //get number of data points
    getline(fl, line);
    unsigned npoints = get_int_token(line);
    //store q
    getline(fl, line);
    q.set(std_to_eigen_vector(get_float_tokens(line, npoints)));
    //store I
    getline(fl, line);
    I.set(std_to_eigen_vector(get_float_tokens(line, npoints)));
    //store err
    getline(fl, line);
    err.set(std_to_eigen_vector(get_float_tokens(line, npoints)));
    //store N
    getline(fl, line);
    N = get_int_token(line);
    //store ndraws
    getline(fl, line);
    ndraws = get_int_token(line);
    //store values
    getline(fl, line);
    values =get_float_tokens(line, ndraws);
}
}

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cout << "error: must provide following arguments" << std::endl;
        std::cout <<  argv[0] << " params.dat data.dat " << std::endl;
        return 1;
    }
    //read input params
    GP::VectorXd X{Eigen::VectorXd()};
    GP::VectorXd y{Eigen::VectorXd()};
    GP::VectorXd err{Eigen::VectorXd()};
    unsigned N,ndraws;
    std::vector<double> values;
    read_input_params(argv[1], X, y, err, N, ndraws, values);
    GP::MatrixXd S(err.get().array().square().matrix().asDiagonal());

    //build GPI object
    auto mean = GP::mean::zero();
    GP::Scalar lambda(1.0);
    auto cov = GP::covariance::squared_exponential(lambda); //1-D covariance
    GP::Scalar tau(1.0);
    GP::Scalar sigma(1.0);
    auto gpi_single = GP::make_gpi(X, y, sigma, tau, mean, cov);
    auto gpi = GP::make_gpi(X, y, S, sigma, tau, N, mean, cov);
    auto gpi_lik = gpi.minus_log_likelihood();

    //read input data and process to matrix form
    const unsigned a_col = 0;
    const unsigned b_col = 1;
    const unsigned sigma_col = 2;
    const unsigned tau_col = 3;
    const unsigned lambda_col = 4;
    const unsigned score_col = 5;
    Eigen::MatrixXd data(read_input_data(argv[2],6));
    
    //perform test on data
    for (unsigned i=0; i<data.rows(); ++i){
        std::cout << " =============== " << i << std::endl;
        assert(data(i,a_col)==0);
        assert(data(i,b_col)==0);
        sigma.set(data(i,sigma_col));
        tau.set(data(i,tau_col));
        lambda.set(data(i,lambda_col));
        double expected = data(i,score_col);
        std::cout << "get" << std::endl;
        double observed = gpi_lik.get();
        std::cout << "done" << std::endl;
        if (std::abs(expected-observed) > 1e-5) {
            std::cout << "discrepancy observed at " << i << " "
                << observed << " " << expected << std::endl;
            return 2;
        }
    }
    return 0;
}
