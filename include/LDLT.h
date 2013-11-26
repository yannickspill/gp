#ifndef LDLT_H
#define LDLT_H

#include "macros.h"
#include "SingleInputCachedVersionTracker.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <assert.h>

//! Build the LDLT (Cholesky) decomposition of a matrix
//  Sigma = LDL^T
//  Only the upper corner of the matrix is used.
template <typename MATRIX>
class LDLT : public SingleInputCachedVersionTracker<MATRIX> {

    typedef SingleInputCachedVersionTracker<MATRIX> P;
   public:
    typedef Eigen::LDLT<typename MATRIX::result_type, Eigen::Upper> result_type;

   private:
    struct Data {
        MATRIX cov_;
        mutable result_type ldlt_;
        Data(MATRIX cov) : cov_(cov) {}
    };
    std::shared_ptr<Data> data_;

   public:
    //! constructor
    LDLT(MATRIX cov) : P(cov), data_(std::make_shared<Data>(cov)) {}

    const result_type& get() const { return get_ldlt(); }

    double get_log_determinant() const {
        return get_ldlt().vectorD().array().abs().log().sum();
    }

    //! solve for Sigma X = B, returning X = Sigma^{-1} B
    template <class Derived>
    auto solve(const Eigen::MatrixBase<Derived>& B)
        const -> decltype(data_->ldlt_.solve(B)) {
        return get_ldlt().solve(B);
    }

   private:
    const result_type& get_ldlt() const {
        if (P::cache_is_invalid()) {
            data_->ldlt_ = result_type(data_->cov_.get());
            assert(data_->ldlt_.info() == Eigen::Success);
            P::set_cache_is_valid();
        }
        return data_->ldlt_;
    }
};

#endif /* LDLT_H */
