#ifndef MATRIX_SUM_H
#define MATRIX_SUM_H

#include "DoubleInputVersionTracker.h"
#include "macros.h"

#include <Eigen/Dense>
#include <iostream>

//! Sum between two Eigen objects
template <class In1Type, class In2Type>
class MatrixSum : public DoubleInputVersionTracker<In1Type, In2Type> {
    typedef DoubleInputVersionTracker<In1Type, In2Type> P;
    In1Type in1_;
    In2Type in2_;

   public:
    //! constructor
    MatrixSum(In1Type in1, In2Type in2)
        : P(in1, in2), in1_(in1), in2_(in2) {}

    typedef decltype(in1_.get() + in2_.get()) result_type;
    result_type get() const { return in1_.get() + in2_.get(); }
};

#endif /* MATRIX_SUM_H */
