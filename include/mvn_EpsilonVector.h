#ifndef MVN_EPSILON_H
#define MVN_EPSILON_H

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

template <class MEANFUNC>
class EpsilonVector {

    MatrixXd X_;
    VectorXd y_;
    MEANFUNC mu_;

   public:
    EpsilonVector(MatrixXd& X, VectorXd& y, MEANFUNC& mu)
        : X_(X), y_(y), mu_(mu) {}

    VectorXd operator()() const { return y_ - get_mean(); }

   private:
    VectorXd get_mean() const {
        VectorXd mean(X_.rows());
        for (unsigned i = 0; i < X_.rows(); i++) mean(i) = mu_(X_.row(i));
        return mean;
    }
};

#endif /* MVN_EPSILON_H */
