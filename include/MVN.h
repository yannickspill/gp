#ifndef MVN_H
#define MVN_H

#include "Matrix.h"

#include <Eigen/Dense>

namespace GP {
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
template <class YVectorType, class MVectorType, class MatrixType> class MVN {

  YVectorType y_;
  MVectorType mu_;
  MatrixType Sigma_;
  decltype(y_ - mu_) eps_;
  decltype(Sigma_.decomposition().cache()) ldlt_;

 public:
  /** Constructor
  * \param [in] X vector of observations with M rows.
  * \param [in] MU mean vector \f$F(\mu)\f$ of size M.
  * \param [in] Sigma : MxM variance-covariance matrix \f$\Sigma\f$.
  * */
  MVN(const YVectorType& y, const MVectorType& mu, const MatrixType& Sigma)
      : y_(y), mu_(mu), Sigma_(Sigma), eps_(y - mu),
        ldlt_(Sigma.decomposition().cache()) {}

  /// return -log(p)
  double get() const { return value().get(); }

  /// return gradient of -log(p) wrt y
  Eigen::VectorXd get_derivative_y() const { return -deriv_mu_value().get(); }
  /// return gradient of -log(p) wrt mu
  Eigen::VectorXd get_derivative_mu() const { return deriv_mu_value().get(); }

  /// return gradient of -log(p) wrt Sigma
  Eigen::MatrixXd get_derivative_Sigma() const {
    return deriv_Sigma_value().get();
  }

  auto value() const -> decltype(0.5 * eps_.transpose() * ldlt_.solve(eps_)
                                 + eps_.rows() / 2. * std::log(2 * M_PI)
                                 + 0.5 * ldlt_.logdet()) {
    // -log(p) = 1/2 * eps^T * Sigma^{-1} * eps
    //           + M/2 * log(2*pi) + 1/2*log(det(Sigma))
    LOG(" mvn value" << std::endl);
    return 0.5 * eps_.transpose() * ldlt_.solve(eps_)
           + eps_.rows() / 2. * std::log(2 * M_PI) + 0.5 * ldlt_.logdet();
  }

  auto deriv_mu_value() const -> decltype(-ldlt_.solve(eps_)) {
    // d(-log(p))/d(MU) = - Sigma^{-1} * epsilon
    LOG("mvn deriv_mu" << std::endl);
    return -ldlt_.solve(eps_);
  }

  auto deriv_Sigma_value() const
      -> decltype(0.5 * (ldlt_.solve(std::declval<MatrixXd>())
                         - ldlt_.solve(eps_) * ldlt_.solve(eps_).transpose())) {
    // d(-log(p))/dSigma = 1/2 (P -  P epsilon transpose(epsilon) P)
    MatrixXd Id(Eigen::MatrixXd::Identity(Sigma_.rows(), Sigma_.cols()));
    auto P = ldlt_.solve(Id);
    auto Peps = ldlt_.solve(eps_);
    return 0.5 * (P - Peps * Peps.transpose());
  }
};
}
#endif /* MVN_H */
