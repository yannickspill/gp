#ifndef LDLT_H
#define LDLT_H

#include "macros.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>

//! Build the LDLT (Cholesky) decomposition of a matrix
//  Sigma = LDL^T
//  Only the upper corner of the matrix is used.
template <typename MATRIX>
class LDLT {
    MATRIX cov_;

   public:
    //! constructor
    LDLT(MATRIX cov) : cov_(cov) {}

    Eigen::LDLT<decltype(cov_.get()), Eigen::Upper> get() const {
        return get_ldlt();
    }

    double get_log_determinant() const {
        return get_ldlt().vectorD().array().abs().log().sum();
    }

    //! solve for Sigma X = B, returning X = Sigma^{-1} B
    Eigen::MatrixXd solve(const Eigen::MatrixXd& B) const {
        return get_ldlt().solve(B);
    }
    Eigen::VectorXd solve(const Eigen::VectorXd& B) const {
        return get_ldlt().solve(B);
    }

   private:
    Eigen::LDLT<decltype(cov_.get()), Eigen::Upper> get_ldlt() const {
        return Eigen::LDLT<decltype(cov_.get()), Eigen::Upper>(cov_.get());
    }
};

#endif /* LDLT_H */
