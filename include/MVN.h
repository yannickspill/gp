#ifndef MVN_H
#define MVN_H

#include "MatrixDifference.h"
#include "LDLT.h"
#include "SolveDecomposedMatrix.h"

#include <Eigen/Dense>

//! Multivariate normal distribution
//FX : F(X)
//lJF : -log(jacobian)
//FM : F(mu)
//Sigma: covariance
template <class VECTORX, class VECTORM, class MATRIX>
class MVN {

    typedef MatrixDifference<VECTORX, VECTORM> MD;

    VECTORX FX_;
    VECTORM FM_;
    double lJF_;
    MATRIX Sigma_;
    LDLT<MATRIX> ldlt_;
    MD eps_;
    SolveDecomposedMatrix<LDLT<MATRIX>, MD> Peps_;

   public:
    MVN(VECTORX FX, VECTORM FM, double lJF, MATRIX Sigma)
        : FX_(FX),
          FM_(FM),
          lJF_(lJF),
          Sigma_(Sigma),
          ldlt_(Sigma_),
          eps_(FX_, FM_),
          Peps_(ldlt_, eps_) {}

    double get() const { return evaluate(); }

    Eigen::VectorXd get_derivative_FX() const { return -deriv_FM(); }
    Eigen::VectorXd get_derivative_FM() const { return deriv_FM(); }

    Eigen::MatrixXd get_derivative_Sigma() const { return deriv_Sigma(); }
    
   private:

    void update() {
        //return values are not used
        eps_.update();
        ldlt_.update();
        Peps_.update();
    }

    double evaluate() const {
        LOG(" mvn eval: eps" << std::endl);
        Eigen::VectorXd epsilon(eps_.get());
        LOG(" mvn eval: Peps" << std::endl);
        Eigen::VectorXd Peps(Peps_.get());
        double exponent = epsilon.transpose()*Peps;
        LOG(" mvn eval: determinant" << std::endl);
        double lnorm = ldlt_.get_log_determinant();
        lnorm = epsilon.rows()*0.5*std::log(2*M_PI) + 0.5*lnorm;
        LOG(" mvn eval: return" << std::endl);
        return lnorm + 0.5 * exponent + lJF_;
    }

    Eigen::VectorXd deriv_FM() const {
        // d(-log(p))/d(FM) = - Sigma^{-1} * epsilon
        LOG(" mvn FM: return Peps" << std::endl);
        return - Peps_.get();
    }

    Eigen::MatrixXd deriv_Sigma() const {
        // d(-log(p))/dSigma = 1/2 (P -  P epsilon transpose(epsilon) P)
        LOG(" mvn sigma: eps" << std::endl);
        Eigen::VectorXd epsilon(eps_.get());
        LOG(" mvn sigma: Peps" << std::endl);
        Eigen::VectorXd Peps(Peps_.get());
        Eigen::MatrixXd Id(
            Eigen::MatrixXd::Identity(epsilon.rows(), epsilon.rows()));
        LOG(" mvn sigma: ldlt solve" << std::endl);
        Eigen::MatrixXd P(ldlt_.solve(Id));
        LOG(" mvn sigma: return" << std::endl);
        return 0.5*( P - Peps*Peps.transpose() );
    }
};

#endif /* MVN_EVALUATE_H */
