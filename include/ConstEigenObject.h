#ifndef CONST_EIGEN_OBJECT_H
#define CONST_EIGEN_OBJECT_H

#include "Scalar.h"
#include <iostream>

//! take an Eigen object and just provide a get() function for it
template <class EigenType>
class ConstEigenObject {
    EigenType X_;

  public:
    explicit ConstEigenObject(const EigenType& X) : X_(X) {}

    typedef EigenType result_type;
    const result_type& get() const {
        return X_;
    }
    unsigned update() const {
        return 0;    // ConstEigenObject not updatable
    }
    bool get_is_function_of(const Scalar&) const {
        return false;
    }
    result_type get_derivative(const Scalar&) const {
        return EigenType::Zero(X_.rows(),X_.cols());
    }
};

#endif /* CONST_EIGEN_OBJECT_H */
