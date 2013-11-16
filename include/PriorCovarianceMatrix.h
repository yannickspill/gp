#ifndef PRIOR_COVARIANCE_MATRIX_H
#define PRIOR_COVARIANCE_MATRIX_H

#include "Scalar.h"
#include <Eigen/Dense>

//! Build a covariance matrix from a covariance function
// Only the upper corner of the matrix is used.
template <class COVFUNC>
class PriorCovarianceMatrix {
    Eigen::MatrixXd X_;
    COVFUNC cov_;

   public:
    //! constructor
    // X : input coordinates (each coord is a RowVectorXd)
    // cov : covariance function, compatible with X's shape.
    PriorCovarianceMatrix(const Eigen::MatrixXd& X, COVFUNC cov)
        : X_(X), cov_(cov) {}

    Eigen::MatrixXd operator()() const { return get_covariance(); }

   private:
    Eigen::MatrixXd get_covariance() const {
        Eigen::MatrixXd covmat(X_.rows(), X_.rows());
        for (unsigned i = 0; i < X_.rows(); i++)
            for (unsigned j = i; j < X_.rows(); j++)
                covmat(i, j) = cov_(X_.row(i), X_.row(j));
        return covmat.selfadjointView<Eigen::Upper>();
    }
};

#endif /* PRIOR_COVARIANCE_MATRIX_H */
