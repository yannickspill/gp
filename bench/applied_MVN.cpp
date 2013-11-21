#include "MVN.h"
#include "ConstEigenObject.h"
#include "macros.h"
#include "benchmark_helpers.h"

#include <Eigen/Dense>

#include <iostream>

typedef ConstEigenObject<Eigen::VectorXd> ConstVec;
typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
typedef MVN<ConstVec, ConstVec, ConstMat> Multivariate;

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
    ConstVec S(get_vector_column(data, 2).array().square());

    // read input params
    std::vector<std::vector<double> > params;
    std::vector<bool> use_deriv;
    read_input_parameters(argv[2], params, use_deriv);
    // process data to Eigen form
    ConstVec mu(get_mean_vector(x_values.get(), params[0]));
    ConstMat Sigma(get_covariance_matrix(x_values.get(), S.get(), params[0]));

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
            mvn.set_FM(ConstVec(get_mean_vector(x_values.get(), param)));
        if (covariance_has_changed(params, i))
            mvn.set_Sigma(ConstMat(get_covariance_matrix(x_values.get(), S.get(), param)));
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
