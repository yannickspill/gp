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
    Scalar a_, b_;

   public:
    Linear1DFunction(Scalar a, Scalar b) : a_(a), b_(b) {}

    double eval(double x) const {
        return get_a_value() * x + get_b_value();
    }
    double eval(Eigen::Matrix<double, 1, 1> x) const {
        return eval(x(0));
    }

   private:
    double get_a_value() const { return a_.get(); }
    double get_b_value() const { return b_.get(); }
};

#endif /* LINEAR1D_FUNCTION_H */
