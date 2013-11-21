#include "MultivariateFNormalSufficient.h"
#include "FNormal.h"
#include "macros.h"
#include "benchmark_helpers.h"

#include <Eigen/Dense>

#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXi;

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
    VectorXd x_values(get_vector_column(data, 0));
    Eigen::RowVectorXd y_values(get_vector_column(data, 1).transpose());
    VectorXd S(get_vector_column(data, 2).array().square());

    // read input params
    std::vector<std::vector<double> > params;
    std::vector<bool> use_deriv;
    read_input_parameters(argv[2], params, use_deriv);
    // process data to Eigen form
    VectorXd mu(get_mean_vector(x_values, params[0]));
    MatrixXd Sigma(get_covariance_matrix(x_values, S, params[0]));

    // create MVN object
    const double JA = 1.0;  // jacobian is 1 for normal distribution
    const unsigned N = 1;  // just one M-dimensional gaussian
    MultivariateFNormalSufficient mvn(y_values, JA, mu, Sigma);

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
            mvn.evaluate_derivative_FM();
            mvn.evaluate_derivative_Sigma();
        }
        mvn.evaluate();
    }
    std::cout << "DONE" << std::endl;
    return 0;
}
