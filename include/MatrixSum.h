#ifndef MATRIX_SUM_H
#define MATRIX_SUM_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>

//! Sum two Eigen objects
template <class LEFTTYPE, class RIGHTTYPE>
class MatrixSum {
    LEFTTYPE l_;
    RIGHTTYPE r_;

   public:
    //! constructor
    MatrixSum(LEFTTYPE l, RIGHTTYPE r) : l_(l), r_(r) {}

    typedef decltype(l_.get() + r_.get()) result_type;
    result_type get() const {return l_.get() + r_.get();}

};

#endif /* MATRIX_SUM_H */
