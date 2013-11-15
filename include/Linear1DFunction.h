#ifndef LINEAR1D_FUNCTION_H
#define LINEAR1D_FUNCTION_H

//! Linear one-dimensional function
/* f(x) = a*x + b
 * a,b : Scalars
 * f(x) : double
 * x : double (or Vector1D)
 */
class Linear1DFunction {
    Scalar* a_, *b_;

   public:
    Linear1DFunction(Scalar* a, Scalar* b) : a_(a), b_(b) {}

    double operator()(double x) { return get_a_value() * x + get_b_value(); }

   private:
    double get_a_value() { return a_->get(); }
    double get_b_value() { return b_->get(); }
};

#endif /* LINEAR1D_FUNCTION_H */
