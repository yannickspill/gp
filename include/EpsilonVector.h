#ifndef EPSILON_VECTOR_H
#define EPSILON_VECTOR_H

#include "DoubleInputVersionTracker.h"
#include "macros.h"

#include <Eigen/Dense>
#include <iostream>
#include <memory>

//! Difference between two wrapped Eigen vectors
template <class Vector1Type, class Vector2Type>
class EpsilonVector : public DoubleInputVersionTracker<Vector1Type,
    Vector2Type> {
    typedef DoubleInputVersionTracker<Vector1Type, Vector2Type> P;
    struct Data {
        Vector1Type in1_;
        Vector2Type in2_;
        Data(Vector1Type in1, Vector2Type in2) : in1_(in1), in2_(in2) {}
    };
    std::shared_ptr<Data> data_;

  public:
    //! constructor
    EpsilonVector(Vector1Type in1, Vector2Type in2)
        : P(in1, in2), data_(std::make_shared<Data>(in1,in2)) {}

    typedef decltype(data_->in1_.get() - data_->in2_.get()) result_type;
    result_type get() const {
        LOG("EpsilonVector: get()" << std::endl);
        return data_->in1_.get() - data_->in2_.get();
    }

    typedef typename Vector1Type::result_type derivative_type;

    derivative_type get_derivative(const Scalar& s) const {
        //d(eps)/ds = d(left)/ds - d(right)/ds
        double rows(data_->in1_.get().rows());
        derivative_type deriv(derivative_type::Zero(rows));
        if (data_->in1_.get_is_function_of(s))
            deriv += data_->in1_.get_derivative(s);
        if (data_->in2_.get_is_function_of(s))
            deriv -= data_->in2_.get_derivative(s);
        return deriv;
    }
};

#endif /* EPSILON_VECTOR_H */
