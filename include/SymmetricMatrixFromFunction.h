#ifndef SYMMETRIC_MATRIX_FROM_FUNCTION_H
#define SYMMETRIC_MATRIX_FROM_FUNCTION_H

#include <Eigen/Dense>

//! Build a dense symmetric matrix from a given function
// maps any two rows of INMATRIX through BIVFUNC to yield a symmetric matrix
// only the upper half of the matrix is filled
template <class INMATRIX, class BIVFUNC>
class SymmetricMatrixFromFunction {
   public:
    //! constructor
    // X : input coordinates
    // cov : covariance function, compatible with X's shape.
    SymmetricMatrixFromFunction(INMATRIX X, BIVFUNC cov)
        : X_(X),
          cov_(cov),
          vmat_(X_.update()),
          vfunc_(cov_.update()),
          version_(0) {}

    typedef Eigen::MatrixXd result_type;
    const result_type& get() const { return build_matrix(); }

    unsigned update() {
        unsigned vmat = X_.update();
        unsigned vfunc = cov_.update();
        if (vmat != vmat_ || vfunc != vfunc_) version_++;
        vmat_=vmat;
        vfunc_=vfunc;
        return version_;
    }

   private:
    const result_type& build_matrix() const {
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
    unsigned vmat_, vfunc_, version_;
    mutable result_type retval_;

};

#endif /* SYMMETRIC_MATRIX_FROM_FUNCTION_H */
