#ifndef MVN_H
#define MVN_H

#include "MatrixDifference.h"
#include "LDLT.h"
#include "SolveDecomposedMatrix.h"

#include <Eigen/Dense>

//! Multivariate normal distribution
/** Probability density function and -log(p) of multivariate normal
 * distribution of an M-variate observation.
 *
 * \f[ p(x|\mu,\Sigma) =
 *  \frac{1}{(2\pi)^{M/2}|\Sigma|^{1/2}}
 *  \exp\left(-\frac{1}{2} (x-\mu)^\top\Sigma^{-1}(x-\mu)  \right)
 *  \f]
 *
 *  \note References:
 *  - Multivariate Likelihood:
 *  Box and Tiao, "Bayesian Inference in Statistical Analysis",
 *  Addison-Wesley publishing company, 1973, pp 423.
 *  - Matrix calculations for derivatives:
 *  Petersen and Pedersen, "The Matrix Cookbook", 2008, matrixcookbook.com
 *
 */
template <class VECTORX, class VECTORM, class MATRIX>
class MVN {

    typedef MatrixDifference<VECTORX, VECTORM> MD;

    VECTORX X_;
    VECTORM MU_;
    MATRIX Sigma_;
    LDLT<MATRIX> ldlt_;
    MD eps_;
    SolveDecomposedMatrix<LDLT<MATRIX>, MD> Peps_;

   public:
    /** Constructor
    * \param [in] X vector of observations with M rows.
    * \param [in] MU mean vector \f$F(\mu)\f$ of size M.
    * \param [in] Sigma : MxM variance-covariance matrix \f$\Sigma\f$.
    * */
    MVN(VECTORX X, VECTORM MU, MATRIX Sigma)
        : X_(X),
          MU_(MU),
          Sigma_(Sigma),
          ldlt_(Sigma_),
          eps_(X_, MU_),
          Peps_(ldlt_, eps_) {}

    /// return -log(p)
    double get() const { return evaluate(); }

    /// return gradient of -log(p) wrt X
    Eigen::VectorXd get_derivative_X() const { return -deriv_MU(); }
    /// return gradient of -log(p) wrt MU
    Eigen::VectorXd get_derivative_MU() const { return deriv_MU(); }

    /// return gradient of -log(p) wrt Sigma
    Eigen::MatrixXd get_derivative_Sigma() const { return deriv_Sigma(); }
    
    //get partial derivative of -log(p) wrt obj
    //template<class OBJ>
    //Eigen::MatrixXd deriv(const OBJ& obj) const {
    //}

   private:

    void update() {
        //return values are not used
        eps_.update();
        ldlt_.update();
        Peps_.update();
    }

    double evaluate() const {
        // -log(p) =
        LOG(" mvn eval: eps" << std::endl);
        Eigen::VectorXd epsilon(eps_.get());
        LOG(" mvn eval: Peps" << std::endl);
        Eigen::VectorXd Peps(Peps_.get());
        double exponent = epsilon.transpose()*Peps;
        LOG(" mvn eval: determinant" << std::endl);
        double lnorm = ldlt_.get_log_determinant();
        lnorm = epsilon.rows()*0.5*std::log(2*M_PI) + 0.5*lnorm;
        LOG(" mvn eval: return" << std::endl);
        return lnorm + 0.5 * exponent;
    }

    Eigen::VectorXd deriv_MU() const {
        // d(-log(p))/d(MU) = - Sigma^{-1} * epsilon
        LOG(" mvn MU: return Peps" << std::endl);
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

#endif /* MVN_H */
