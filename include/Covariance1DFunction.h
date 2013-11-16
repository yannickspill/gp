#ifndef COVARIANCE1D_FUNCTION_H
#define COVARIANCE1D_FUNCTION_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>
#include <math.h>

//! One-dimensional squared exponential covariance function
/* f(x,y) = tau^2*exp( -1/2 * ((x-y)/lambda)^alpha )
 * tau,lambda : Scalars which have to be positive 
 *      (lower bound will be set to zero if that's not the case)
 * f(x) : double
 * alpha : unsigned (alpha = 2 by default)
 * x,y : doubles (or one Vector2D)
 */
class Covariance1DFunction {
    Scalar tau_, lambda_;
    unsigned alpha_;

   public:
    Covariance1DFunction(Scalar tau, Scalar lambda, unsigned alpha = 2)
        : tau_(tau), lambda_(lambda), alpha_(alpha) {
            if (tau_.get_lower() < 0) tau_.set_lower(0);
            if (lambda_.get_lower() < 0) lambda_.set_lower(0);
        }

    double operator()(double x, double y) const {
        double dist = std::abs(x-y);
        double ret = dist/get_lambda_value();
        if (alpha_ == 2) {
            ret = SQUARE(ret);
        } else {
            ret = std::pow(ret, alpha_);
        }
        return SQUARE(get_tau_value()) * std::exp(-0.5 * ret);
    }
    double operator()(Eigen::Vector2d x) const {
        return operator()(x(0), x(1));
    }

   private:
    double get_tau_value() const { return tau_.get(); }
    double get_lambda_value() const { return lambda_.get(); }
};

#endif /* COVARIANCE1D_FUNCTION_H */
