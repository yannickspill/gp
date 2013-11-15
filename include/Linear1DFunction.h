#ifndef LINEAR1D_FUNCTION_H
#define LINEAR1D_FUNCTION_H

#include "Scalar.h"
#include <Eigen/Dense>

//! Linear one-dimensional function
/* f(x) = a*x + b
 * a,b : Scalars
 * f(x) : double
 * x : double (or Vector1D)
 */
class Linear1DFunction {
    Scalar *a_, *b_;

   public:
    Linear1DFunction(Scalar* a, Scalar* b) : a_(a), b_(b) {}

    double operator()(double x) const {
        return get_a_value() * x + get_b_value();
    }

    double operator()(Eigen::Matrix<double, 1, 1> x) const {
        return (*this)(x(0));
    }

   private:
    double get_a_value() const { return a_->get(); }
    double get_b_value() const { return b_->get(); }
};

#endif /* LINEAR1D_FUNCTION_H */
