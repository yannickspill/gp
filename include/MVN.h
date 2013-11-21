#ifndef MVN_H
#define MVN_H

#include "MatrixDifference.h"
#include "LDLT.h"
#include "mvn_MVNEvaluate.h"

#include <Eigen/Dense>

//! Multivariate normal distribution
//FX : F(X)
//lJF : -log(jacobian)
//FM : F(mu)
//Sigma: covariance
template <class VECTORX, class VECTORM, class MATRIX>
class MVN {

    VECTORX FX_;
    VECTORM FM_;
    double lJF_;
    MATRIX Sigma_;

    typedef MatrixDifference<VECTORX, VECTORM> MD;

   public:
    MVN(VECTORX FX, VECTORM FM, double lJF, MATRIX Sigma)
        : FX_(FX), FM_(FM), lJF_(lJF), Sigma_(Sigma) {}

    double get() const { return evaluate(); }

    Eigen::VectorXd get_derivative_FX() const { return -deriv_FM(); }
    Eigen::VectorXd get_derivative_FM() const { return deriv_FM(); }

    Eigen::MatrixXd get_derivative_Sigma() const { return deriv_Sigma(); }

    void set_FX(VECTORX FX) { FX_ = FX; }
    void set_FM(VECTORM FM) { FM_ = FM; }
    void set_lJF(double lJF) { lJF_ = lJF; }
    void set_Sigma(MATRIX Sigma) { Sigma_ = Sigma; }

   private:
    double evaluate() const {
        //first we transform the inputs to something MVNEvaluate can process
        MD eps(FX_, FM_);
        LDLT<MATRIX> ldlt(Sigma_);
        Eigen::VectorXd epsilon(eps.get());
        Eigen::VectorXd Peps(ldlt.solve(epsilon));
        double exponent = epsilon.transpose()*Peps;
        double lnorm = ldlt.get_log_determinant();
        lnorm = epsilon.rows()*0.5*std::log(2*M_PI) + 0.5*lnorm;
        return lnorm + 0.5 * exponent + lJF_;
    }

    Eigen::VectorXd deriv_FM() const {
        // d(-log(p))/d(FM) = - Sigma^{-1} * epsilon
        MD eps(FX_, FM_);
        LDLT<MATRIX> ldlt(Sigma_);
        Eigen::VectorXd epsilon(eps.get());
        Eigen::VectorXd Peps(ldlt.solve(epsilon));
        return -Peps;
    }

    Eigen::MatrixXd deriv_Sigma() const {
        // d(-log(p))/dSigma = 1/2 (P -  P epsilon transpose(epsilon) P)
        MD eps(FX_, FM_);
        LDLT<MATRIX> ldlt(Sigma_);
        Eigen::VectorXd epsilon(eps.get());
        Eigen::VectorXd Peps(ldlt.solve(epsilon));
        Eigen::MatrixXd Id(
            Eigen::MatrixXd::Identity(epsilon.rows(), epsilon.rows()));
        Eigen::MatrixXd P(ldlt.solve(Id));
        return 0.5*( P - Peps*Peps.transpose() );
    }
};

#endif /* MVN_EVALUATE_H */
