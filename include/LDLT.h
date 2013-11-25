#ifndef LDLT_H
#define LDLT_H

#include "macros.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <assert.h>

//! Build the LDLT (Cholesky) decomposition of a matrix
//  Sigma = LDL^T
//  Only the upper corner of the matrix is used.
template <typename MATRIX>
class LDLT {

   public:
    typedef Eigen::LDLT<typename MATRIX::result_type, Eigen::Upper> result_type;

   private:
    MATRIX cov_;
    unsigned vmat_, version_;
    mutable bool cache_invalid_;
    mutable result_type ldlt_;

   public:
    //! constructor
    LDLT(MATRIX cov)
        : cov_(cov), vmat_(cov.update()), version_(0), cache_invalid_(true) {}

    const result_type& get() const { return get_ldlt(); }

    double get_log_determinant() const {
        return get_ldlt().vectorD().array().abs().log().sum();
    }

    void set_matrix(MATRIX cov) {
        cov_ = cov;
        vmat_ = cov_.update();
        cache_invalid_=true;
        version_++;
    }

    unsigned update() {
        unsigned vmat = cov_.update();
        if (vmat != vmat_)
        {
            cache_invalid_=true; //allows lazy updating of ldlt
            version_++;
        }
        vmat_ = vmat;
        return version_;
    }

    //! solve for Sigma X = B, returning X = Sigma^{-1} B
    template <class Derived>
    auto solve(const Eigen::MatrixBase<Derived>& B)
        const -> decltype(ldlt_.solve(B)) {
        return get_ldlt().solve(B);
    }

   private:
    const result_type& get_ldlt() const {
        if (cache_invalid_) {
            ldlt_ = result_type(cov_.get());
            assert(ldlt_.info() == Eigen::Success);
            cache_invalid_=false;
        }
        return ldlt_;
    }
};

#endif /* LDLT_H */
