#ifndef VECTOR_FROM_FUNCTION_H
#define VECTOR_FROM_FUNCTION_H

#include <Eigen/Dense>

//! map the rows of INMATRIX through UNIFUNC to a VECTOR
template <class INMATRIX, class UNIFUNC>
class VectorFromFunction {

   public:
    VectorFromFunction(INMATRIX X, UNIFUNC mu) : X_(X), mu_(mu),
    vmat_(X_.update()), vfunc_(mu_.update()), version_(0) {}

    typedef Eigen::VectorXd result_type;
    const result_type& get() const { return build_vector(); }

    unsigned update() {
        unsigned vmat = X_.update();
        unsigned vfunc = mu_.update();
        if (vmat != vmat_ || vfunc != vfunc_) version_++;
        vmat_=vmat;
        vfunc_=vfunc;
        return version_;
    }

   private:
    const result_type& build_vector() const {
        auto Xmat(X_.get());
        retval_ = Eigen::VectorXd(Xmat.rows());
        for (unsigned i = 0; i < Xmat.rows(); i++)
            retval_(i) = mu_.eval(Xmat.row(i));
        return retval_;
    }

   private:
    INMATRIX X_;
    UNIFUNC mu_;
    unsigned vmat_, vfunc_, version_;
    mutable result_type retval_;

};

#endif /* VECTOR_FROM_FUNCTION */
