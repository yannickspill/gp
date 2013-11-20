#ifndef SYMMETRIC_MATRIX_FROM_FUNCTION_H
#define SYMMETRIC_MATRIX_FROM_FUNCTION_H

#include <Eigen/Dense>

//! Build a dense symmetric matrix from a given function
// maps any two rows of INMATRIX through BIVFUNC to yield a symmetric matrix
// only the upper half of the matrix is filled
template <class INMATRIX, class BIVFUNC>
class SymmetricMatrixFromFunction {
    INMATRIX X_;
    BIVFUNC cov_;

   public:
    //! constructor
    // X : input coordinates
    // cov : covariance function, compatible with X's shape.
    SymmetricMatrixFromFunction(INMATRIX X, BIVFUNC cov)
        : X_(X), cov_(cov) {}

    typedef Eigen::MatrixXd result_type;
    result_type get() const { return build_matrix(); }

   private:
    result_type build_matrix() const {
        auto Xmat(X_.get());
        result_type covmat(Xmat.rows(), Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            for (unsigned j = i; j < Xmat.rows(); j++)
                covmat(i, j) = cov_.eval(Xmat.row(i), Xmat.row(j));
        return covmat;
    }
};

#endif /* SYMMETRIC_MATRIX_FROM_FUNCTION_H */
