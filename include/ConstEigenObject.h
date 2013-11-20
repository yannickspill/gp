#ifndef CONST_EIGEN_OBJECT_H
#define CONST_EIGEN_OBJECT_H

#include <iostream>

//! take an Eigen object and just provide a get() function for it
template <class EIGENTYPE>
class ConstEigenObject {
    EIGENTYPE X_;

   public:
    explicit ConstEigenObject(const EIGENTYPE& X) : X_(X) {}

    typedef EIGENTYPE result_type;
    EIGENTYPE get() const { return X_; }
};

#endif /* CONST_EIGEN_OBJECT_H */
