#ifndef MATRIX_DIFFERENCE_H
#define MATRIX_DIFFERENCE_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>
#include <iostream>

//! Difference between two Eigen objects
template <class LEFTTYPE, class RIGHTTYPE>
class MatrixDifference {
    LEFTTYPE l_;
    RIGHTTYPE r_;

   public:
    //! constructor
    MatrixDifference(LEFTTYPE l, RIGHTTYPE r) : l_(l), r_(r) {}

    typedef decltype(l_.get()-r_.get()) result_type;
    result_type get() const {return l_.get() - r_.get();}

};

#endif /* MATRIX_DIFFERENCE_H */
