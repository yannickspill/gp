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
   public:
    //! constructor
    LDLT(MATRIX cov) : cov_(cov), vmat_(cov.update()), version_(0) {}

    typedef Eigen::LDLT<typename MATRIX::result_type, Eigen::Upper>
        result_type;

    const result_type& get() const { return get_ldlt(); }

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

    unsigned update() {
        unsigned vmat = cov_.update();
        if (vmat != vmat_) version_++;
        vmat_=vmat;
        return version_;
    }

   private:
    const result_type& get_ldlt() const {
        ldlt_ = result_type(cov_.get());
        return ldlt_;
    }

   private:
    MATRIX cov_;
    unsigned vmat_, version_;
    mutable result_type ldlt_;

};

#endif /* LDLT_H */
