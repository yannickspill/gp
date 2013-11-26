#ifndef MATRIX_PRODUCT_H
#define MATRIX_PRODUCT_H

#include "DoubleInputVersionTracker.h"
#include "macros.h"

#include <Eigen/Dense>
#include <iostream>
#include <memory>

//! Product between two Eigen objects
template <class In1Type, class In2Type>
class MatrixProduct : public DoubleInputVersionTracker<In1Type, In2Type> {
    typedef DoubleInputVersionTracker<In1Type, In2Type> P;
    struct Data {
        In1Type in1_;
        In2Type in2_;
        Data(In1Type in1, In2Type in2) : in1_(in1), in2_(in2) {}
    };
    std::shared_ptr<Data> data_;

   public:
    //! constructor
    MatrixProduct(In1Type in1, In2Type in2)
        : P(in1, in2), data_(std::make_shared<Data>(in1,in2)) {}

    typedef decltype(data_->in1_.get() * data_->in2_.get()) result_type;
    result_type get() const { return data_->in1_.get() * data_->in2_.get(); }
};

#endif /* MATRIX_PRODUCT_H */
