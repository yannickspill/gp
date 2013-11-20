#ifndef MATRIX_PRODUCT_H
#define MATRIX_PRODUCT_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>

//! Product of two Eigen objects (or Eigen and Scalar)
template <class LEFTTYPE, class RIGHTTYPE>
class MatrixProduct {
    LEFTTYPE l_;
    RIGHTTYPE r_;

   public:
    //! constructor
    MatrixProduct(LEFTTYPE l, RIGHTTYPE r) : l_(l), r_(r) {}

    typedef Eigen::MatrixXd result_type;

    result_type get() const {return l_.get() * r_.get();}

};

#endif /* MATRIX_PRODUCT_H */
