#ifndef MATRIX_SUM_H
#define MATRIX_SUM_H

#include "DoubleInputVersionTracker.h"
#include "macros.h"

#include <Eigen/Dense>
#include <iostream>
#include <memory>

//! Sum between two Eigen objects
template <class Matrix1Type, class Matrix2Type>
class MatrixSum : public DoubleInputVersionTracker<Matrix1Type, Matrix2Type> {
    typedef DoubleInputVersionTracker<Matrix1Type, Matrix2Type> P;
    struct Data {
        Matrix1Type in1_;
        Matrix2Type in2_;
        Data(Matrix1Type in1, Matrix2Type in2) : in1_(in1), in2_(in2) {}
    };
    std::shared_ptr<Data> data_;

  public:
    //! constructor
    MatrixSum(Matrix1Type in1, Matrix2Type in2)
        : P(in1, in2), data_(std::make_shared<Data>(in1,in2)) {}

    typedef decltype(data_->in1_.get() + data_->in2_.get()) result_type;
    result_type get() const {
        return data_->in1_.get() + data_->in2_.get();
    }
};

#endif /* MATRIX_SUM_H */
