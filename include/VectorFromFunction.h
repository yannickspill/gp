#ifndef VECTOR_FROM_FUNCTION_H
#define VECTOR_FROM_FUNCTION_H

#include <Eigen/Dense>

//! map the rows of INMATRIX through UNIFUNC to a VECTOR
template <class INMATRIX, class UNIFUNC>
class VectorFromFunction {

    INMATRIX X_;
    UNIFUNC mu_;

   public:
    VectorFromFunction(INMATRIX X, UNIFUNC mu) : X_(X), mu_(mu) {}

    typedef Eigen::VectorXd result_type;
    result_type get() const { return build_vector(); }

   private:
    result_type build_vector() const {
        auto Xmat(X_.get());
        result_type vec(Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            vec(i) = mu_.eval(Xmat.row(i));
        return vec;
    }
};

#endif /* VECTOR_FROM_FUNCTION */
