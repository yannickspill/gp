#ifndef LDLT_H
#define LDLT_H

#include "macros.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>

//! Build the LDLT (Cholesky) decomposition of a matrix
//  Sigma = LDL^T
//  where
//  W is a prior covariance matrix
//  sigma is a Scalar
//  S is a data covariance matrix
//  Only the upper corner of the matrix is used.
template <typename COVMAT>
class LDLT {
    COVMAT cov_;

   public:
    //! constructor
    LDLT(COVMAT cov) : cov_(cov) {}

    Eigen::LDLT<Eigen::MatrixXd, Eigen::Upper> operator()() const {
        return get_ldlt();
    }

    double get_minus_log_determinant() const {
        return -get_ldlt().vectorD().array().abs().log().sum();
    }

    //! solve for Sigma X = B, returning X = Sigma^{-1} B
    Eigen::MatrixXd solve(const Eigen::MatrixXd& B) const {
        return get_ldlt().solve(B);
    }

   private:
    Eigen::LDLT<Eigen::MatrixXd, Eigen::Upper> get_ldlt() const {
        return Eigen::LDLT<Eigen::MatrixXd, Eigen::Upper>(cov_());
    }
};

#endif /* LDLT_H */
