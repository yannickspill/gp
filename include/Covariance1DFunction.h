#ifndef COVARIANCE1D_FUNCTION_H
#define COVARIANCE1D_FUNCTION_H

#include "Scalar.h"
#include "DoubleInputVersionTracker.h"
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
class Covariance1DFunction : public DoubleInputVersionTracker<Scalar, Scalar> {
    typedef DoubleInputVersionTracker<Scalar, Scalar> P;
    Scalar tau_, lambda_;
    unsigned alpha_;

   public:
    Covariance1DFunction(Scalar tau, Scalar lambda, unsigned alpha = 2)
        : P(tau,lambda),
          tau_(tau),
          lambda_(lambda),
          alpha_(alpha) {
            if (tau_.get_lower() < 0) tau_.set_lower(0);
            if (lambda_.get_lower() < 0) lambda_.set_lower(0);
        }

    double eval(double x, double y) const {
        const double dist = std::abs(x-y);
        double ret = dist/get_lambda_value();
        if (alpha_ == 2) {
            ret = SQUARE(ret);
        } else {
            ret = std::pow(ret, alpha_);
        }
        return SQUARE(get_tau_value()) * std::exp(-0.5 * ret);
    }
    double eval(Eigen::Matrix<double, 1, 1> x,
                      Eigen::Matrix<double, 1, 1> y) const {
        return eval(x(0), y(0));
    }

    double eval_derivative(double x, double y, const Scalar& s) const {
        const double tauval = get_tau_value();
        if (tauval == 0.) return 0; //all derivatives will be zero
                                    // except lambda==0 not considered here
        if (s == tau_) {
            // d[w(x1,x2)]/dtau = 2/tau*w(x1,x2)
            return eval(x,y)*2./tauval;
        } else if (s == lambda_) {
            // d[w(x,x')]/dlambda
            //= w(x,x') *( alpha |x'-x|^alpha/(2 lambda^{alpha+1}))
            const double lambdaval = get_lambda_value();
            const double ratio(std::abs(x-y)/lambdaval);
            return eval(x, y) * alpha_ * std::pow(ratio, alpha_) /
                   (2. * lambdaval);
        } else {
            return 0;
        }
    }

    double eval_derivative(Eigen::Matrix<double, 1, 1> x,
                           Eigen::Matrix<double, 1, 1> y,
                           const Scalar& s) const {
        return eval_derivative(x(0),y(0),s);
    }

   private:
    double get_tau_value() const { return tau_.get(); }
    double get_lambda_value() const { return lambda_.get(); }
};

#endif /* COVARIANCE1D_FUNCTION_H */
