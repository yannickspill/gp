#ifndef CONST_EIGEN_OBJECT_H
#define CONST_EIGEN_OBJECT_H

#include "Scalar.h"
#include <iostream>

//! take an Eigen object and just provide a get() function for it
template <class EIGENTYPE>
class ConstEigenObject {
    EIGENTYPE X_;

   public:
    explicit ConstEigenObject(const EIGENTYPE& X) : X_(X) {}

    typedef EIGENTYPE result_type;
    const result_type& get() const { return X_; }
    unsigned update() const { return 0; } // ConstEigenObject not updatable
    bool get_is_function_of(const Scalar&) { return false; }
};

#endif /* CONST_EIGEN_OBJECT_H */
