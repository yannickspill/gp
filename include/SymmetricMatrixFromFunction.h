#ifndef SYMMETRIC_MATRIX_FROM_FUNCTION_H
#define SYMMETRIC_MATRIX_FROM_FUNCTION_H

#include "DoubleInputVersionTracker.h"

#include <Eigen/Dense>

//! Build a dense symmetric matrix from a given function
// maps any two rows of INMATRIX through BIVFUNC to yield a symmetric matrix
// only the upper half of the matrix is filled
template <class INMATRIX, class BIVFUNC>
class SymmetricMatrixFromFunction
    : public DoubleInputVersionTracker<INMATRIX, BIVFUNC> {
    typedef DoubleInputVersionTracker<INMATRIX, BIVFUNC> P;

   public:
    //! constructor
    // X : input coordinates
    // cov : covariance function, compatible with X's shape.
    SymmetricMatrixFromFunction(INMATRIX X, BIVFUNC cov)
        : P(X, cov), X_(X), cov_(cov) {}

    typedef Eigen::MatrixXd result_type;
    const result_type& get() const {
        auto Xmat(X_.get());
        retval_ = Eigen::MatrixXd(Xmat.rows(), Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            for (unsigned j = i; j < Xmat.rows(); j++)
                retval_(i, j) = cov_.eval(Xmat.row(i), Xmat.row(j));
        return retval_;
    }

   private:
    INMATRIX X_;
    BIVFUNC cov_;
    mutable result_type retval_;
};

#endif /* SYMMETRIC_MATRIX_FROM_FUNCTION_H */
