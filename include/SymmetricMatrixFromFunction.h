#ifndef SYMMETRIC_MATRIX_FROM_FUNCTION_H
#define SYMMETRIC_MATRIX_FROM_FUNCTION_H

#include "DoubleInputVersionTracker.h"

#include <Eigen/Dense>
#include <memory>

//! Build a dense symmetric matrix from a given function
// maps any two rows of MatrixType through BivariateFunctionType to yield a symmetric matrix
template <class MatrixType, class BivariateFunctionType>
class SymmetricMatrixFromFunction
    : public DoubleInputVersionTracker<MatrixType, BivariateFunctionType> {
    typedef DoubleInputVersionTracker<MatrixType, BivariateFunctionType> P;

  public:
    typedef Eigen::MatrixXd result_type;
    static const unsigned RowsAtCompileTime = MatrixType::RowsAtCompileTime;
    static const unsigned ColsAtCompileTime = MatrixType::RowsAtCompileTime;

    //! constructor
    // X : input coordinates
    // cov : covariance function, compatible with X's shape.
    SymmetricMatrixFromFunction(MatrixType X, BivariateFunctionType cov)
        : P(X, cov), data_(std::make_shared<Data>(X,cov)) {}

    const result_type& get() const {
        auto Xmat(data_->X_.get());
        data_->retval_ = Eigen::MatrixXd(Xmat.rows(), Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            for (unsigned j = i; j < Xmat.rows(); j++) {
                data_->retval_(i, j) =
                    data_->cov_.eval(Xmat.row(i), Xmat.row(j));
                if (j>i) data_->retval_(j,i) = data_->retval_(i,j);
            }
        return data_->retval_;
    }

    const result_type& get_derivative(const Scalar& s) const {
        auto Xmat(data_->X_.get());
        data_->deriv_ = Eigen::MatrixXd(Xmat.rows(), Xmat.rows());
        for (unsigned i=0; i< Xmat.rows(); i++)
            for (unsigned j = i; j < Xmat.rows(); j++) {
                data_->deriv_(i, j) =
                    data_->cov_.eval_derivative(Xmat.row(i), Xmat.row(j), s);
                if (j > i) data_->deriv_(j, i) = data_->deriv_(i, j);
            }
        return data_->deriv_;
    }

  private:
    struct Data {
        MatrixType X_;
        BivariateFunctionType cov_;
        mutable result_type retval_, deriv_;
        Data(MatrixType X, BivariateFunctionType cov) : X_(X), cov_(cov) {}
    };
    std::shared_ptr<Data> data_;
};

#endif /* SYMMETRIC_MATRIX_FROM_FUNCTION_H */
