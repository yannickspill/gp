#ifndef VECTOR_FROM_FUNCTION_H
#define VECTOR_FROM_FUNCTION_H

#include "DoubleInputVersionTracker.h"

#include <Eigen/Dense>

//! map the rows of INMATRIX through UNIFUNC to a VECTOR
template <class INMATRIX, class UNIFUNC>
class VectorFromFunction : public DoubleInputVersionTracker<INMATRIX, UNIFUNC> {
    typedef DoubleInputVersionTracker<INMATRIX, UNIFUNC> P;

   public:
    VectorFromFunction(INMATRIX X, UNIFUNC mu) : P(X, mu), X_(X), mu_(mu) {}

    typedef Eigen::VectorXd result_type;
    const result_type& get() const {
        auto Xmat(X_.get());
        retval_ = Eigen::VectorXd(Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            retval_(i) = mu_.eval(Xmat.row(i));
        return retval_;
    }

   private:
    INMATRIX X_;
    UNIFUNC mu_;
    mutable result_type retval_;
};

#endif /* VECTOR_FROM_FUNCTION */
