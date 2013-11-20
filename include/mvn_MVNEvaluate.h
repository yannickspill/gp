#ifndef MVN_EVALUATE_H
#define MVN_EVALUATE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <Eigen/Dense>
#include <iostream>

//! return minus log of a multivariate normal PDF
template <class VECTOR, class DECOMP>
class MVNEvaluate {

    VECTOR eps_;
    DECOMP ldlt_;

   public:
    MVNEvaluate(VECTOR eps, DECOMP ldlt)
        : eps_(eps), ldlt_(ldlt) {}

    typedef double result_type;
    result_type get() const { return evaluate(); }

   private:
    result_type evaluate() const {
        Eigen::VectorXd epsilon(eps_.get());
        Eigen::VectorXd Seps(ldlt_.solve(epsilon));
        double exponent = epsilon.transpose()*Seps;
        double lnorm = ldlt_.get_log_determinant();
        lnorm = epsilon.rows()*0.5*std::log(2*M_PI) + 0.5*lnorm;
        return lnorm + 0.5 * exponent;
    }
};

#endif /* MVN_EVALUATE_H */
