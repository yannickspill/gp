#ifndef VECTOR_FROM_FUNCTION_H
#define VECTOR_FROM_FUNCTION_H

#include "DoubleInputVersionTracker.h"

#include <Eigen/Dense>
#include <memory>

//! map the rows of INMATRIX through UNIFUNC to a VECTOR
template <class INMATRIX, class UNIFUNC>
class VectorFromFunction : public DoubleInputVersionTracker<INMATRIX, UNIFUNC> {
    typedef DoubleInputVersionTracker<INMATRIX, UNIFUNC> P;

   public:
    VectorFromFunction(INMATRIX X, UNIFUNC mu)
        : P(X, mu), data_(std::make_shared<Data>(X, mu)) {}

    typedef Eigen::VectorXd result_type;
    const result_type& get() const {
        auto Xmat(data_->X_.get());
        data_->retval_ = Eigen::VectorXd(Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            data_->retval_(i) = data_->mu_.eval(Xmat.row(i));
        return data_->retval_;
    }

    result_type get_derivative(const Scalar& s) const {
        auto Xmat(data_->X_.get());
        Eigen::VectorXd retval(Xmat.rows());
        for (unsigned i=0; i < Xmat.rows(); i++)
            retval(i) = data_->mu_.eval_derivative(Xmat.row(i),s);
        return retval;
    }

   private:
    struct Data {
        INMATRIX X_;
        UNIFUNC mu_;
        mutable result_type retval_;
        Data(INMATRIX X, UNIFUNC mu) : X_(X), mu_(mu) {}
    };
    std::shared_ptr<Data> data_;
};

#endif /* VECTOR_FROM_FUNCTION */
