#ifndef POSTERIOR_COVARIANCE_MATRIX_H
#define POSTERIOR_COVARIANCE_MATRIX_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>

//! Build a covariance matrix such that
//  Sigma = W + sigma^2 * S
//  where
//  W is a prior covariance matrix
//  sigma is a Scalar
//  S is a data covariance matrix
//  Only the upper corner of the matrix is used.
template <class COVMAT>
class PosteriorCovarianceMatrix {
    Eigen::MatrixXd S_;
    Scalar sigma_;
    COVMAT cov_;

   public:
    //! constructor
    // X : input coordinates (each coord is a RowVectorXd)
    // cov : covariance function, compatible with X's shape.
    PosteriorCovarianceMatrix(const Eigen::MatrixXd& S, Scalar sigma,
            COVMAT cov) : S_(S), sigma_(sigma), cov_(cov) {}

    Eigen::MatrixXd operator()() const { return get_covariance(); }

   private:
    Eigen::MatrixXd get_covariance() const {
        return cov_()+SQUARE(sigma_.get())*S_;
    }
};

#endif /* POSTERIOR_COVARIANCE_MATRIX_H */
