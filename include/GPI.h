#ifndef GPI_H
#define GPI_H

#include "macros.h"
#include "Functor.h"
#include "Scalar.h"
#include "Matrix.h"
#include "MVN.h"

namespace GP {

//! Gaussian Process Interpolation factory
/** This class provides methods to perform bayesian interpolation/smoothing of
 * data using a gaussian process prior on the function to be interpolated. A
 * gaussian process \f$f\f$ is a collection of random variables, any finite
 * number of which have a joint Normal distribution (Rasmussen, 2005, def.
 * 2.1).
 * \f[ f \sim \mathcal{GP}(m, k)
 * \qquad  m : \mathcal{M}_{1,d}(\mathbb{K}) \to \mathbb{R}
 * \quad k : \mathcal{M}_{1,d}(\mathbb{K})\times\mathcal{M}_{1,d}(\mathbb{K})
 *              \to \mathbb{R}
 * \f]
 * \f[ \Longleftrightarrow
 *      \forall M>0 \quad \forall \mathbf{X} \in \mathcal{M}_{M,d}(\mathbb{K})
 *     \quad \mathbf{f}(X) \sim \mathcal{N}(\mathbf{m}(\mathbf{X}),
 * \sigma^2\tau^2\mathbf{K}(\mathbf{X},\mathbf{X}))
 * \f]
 * \f[
       \mathbf{X}^\top \equiv
              \begin{pmatrix} \mathbf{x}_1 \cdots \mathbf{x}_M\end{pmatrix}
 *     \quad \forall i,j \in\llbracket 1,M \rrbracket
 *     \quad \mathbf{m}(\mathbf{X})_i \equiv m(\mathbf{x}_i)
 *     \quad \mathbf{K}(\mathbf{X},\mathbf{Y})_{ij}
 *           \equiv k(\mathbf{x}_i,\mathbf{y}_j)
 * \f]
 * \noindent where \f$k\f$ is a covariance function, and \f$\sigma,\tau\f$ are
 * Scalars used as proportionality constants.
 *
 * The inputs are \f$ M \f$ noisy mean observations \f$ \mathbf{y} \f$ of \f$ f
 * \f$ at \f$ \mathbf{X} \f$, a Scalar parameter \f$\sigma\f$,
 * a sample variance matrix \f$\mathbf{S}\f$ such that
 *
 * \f[ \mathbf{y} \sim \mathcal{N}\left(\mathbf{f}(\mathbf{X}),\sigma^2
 * \mathbf{S}\right) \f]
 *
 * \noindent the sample size \f$n_r\f$ for the mean and variance of the
 * observations, and a parametric family of prior mean/covariance functions
 * \f$m\f$ and \f$k\f$, these parameters can be optimized to give the most
 * probable posterior mean  and covariance functions.
 *
 * \note A simplified constructor is available when only one observation
 * \f$\mathbf{y}\f$ is made and no sample variance matrix is available. it is
 * equivalent to setting \f$n_r=1\f$ and \f$\mathbf{S}=\mathbf{1}_M\f$
 *
 * Once the parameters have been optimized, the posterior mean function
 * \f$\mathbb{E}[f]\f$ and the posterior covariance function \f$\mathbb{V}[f]\f$
 * can be evaluated at an arbitrary number of points
 * \f$\mathbf{X}^\star\in\mathcal{M}_{N,d}(\mathbb{K})\f$.
 *
 * \tparam XType the type of \f$\mathbf{X}\f$
 * \tparam YType the type of \f$\mathbf{y}\f$
 * \tparam SType the type of \f$\mathbf{S}\f$
 * \tparam MeanFunctionType the type of \f$m\f$
 * \tparam CovarianceFunctionType the type of \f$k\f$
 */
template <class XType, class YType, class SType, class MeanFunctionType,
          class CovarianceFunctionType>
class GPI {

  XType X_;
  YType y_;
  SType S_;
  unsigned nr_;
  Scalar sigma_, tau_;
  MeanFunctionType mean_;
  CovarianceFunctionType cov_;
  internal::MatrixFromUnivariateFunctor<MeanFunctionType, XType> mx_;
  typedef internal::SymmetricMatrixFromBivariateFunctor
      <CovarianceFunctionType, XType> Ktype;
  decltype(sigma_* sigma_
           *(S_ / nr_ + tau_* tau_* MatrixXd::SymmetricApply(cov_, X_))) Omega_;
  decltype(make_mvn(y_, mx_, Omega_)) mvn_lik_;

 public:
  ///\name Constructors for the Gaussian process
  ///@{
  /**Default constructor
   * \param [in] X \f$\mathbf{X}\f$ A matrix in which each row is a
   * \f$d\f$-dimensional input, corresponding to the abscissa of each of the
   * \f$M\f$ observations.
   * \param [in] y \f$\mathbf{y}\f$ vector of noisy mean observations
   * \param [in] S \f$\mathbf{S}\f$ matrix proportional to the sample variance
   * matrix of these observations
   * \param [in] nr \f$n_r\f$ sample size for \f$\mathbf{y}\f$ and
   * \f$\mathbf{S}\f$
   * \param[in] sigma \f$\sigma\f$ Scalar used as proportionality factor to
   * \f$\mathbf{S}\f$.
   * \param[in] tau \f$\tau\f$ Scalar used as proportionality factor to
   * \f$\mathbf{K}\f$.
   * \param[in] m the mean function \f$m\f$
   * \param[in] k the covariance function \f$k\f$
   */
  GPI(const XType& X, const YType& y, const SType& S, Scalar sigma, Scalar tau,
      unsigned nr, const MeanFunctionType& m, const CovarianceFunctionType& k)
      : X_(X), y_(y), S_(S), nr_(nr), sigma_(sigma), tau_(tau), mean_(m),
        cov_(k), mx_(VectorXd::Apply(mean_, X_)),
        Omega_(sigma_ * sigma_
               * (S_ / nr_ + tau_ * tau_ * MatrixXd::SymmetricApply(cov_, X_))),
        mvn_lik_(make_mvn(y_, mx_, Omega_)) {
            CHECK(X_.rows() == y_.rows(), "X and y should have same row count");
            CHECK(S_.rows() == y_.rows(), "S and y should have same row count");
            CHECK(S_.rows() == S_.cols(), "S should be square");
            CHECK(S_.rows() == Omega_.cols(), "Weird error 1");
            CHECK(Omega_.rows() == Omega_.cols(), "Weird error 2");
        }

  /** Simplified constructor
   * \param [in] X \f$\mathbf{X}\f$ A matrix in which each row is a
   * \f$d\f$-dimensional input, corresponding to the abscissa of each of the
   * \f$M\f$ observations.
   * \param [in] y \f$\mathbf{y}\f$ vector of noisy mean observations
   * \param[in] sigma \f$\sigma\f$ Scalar used as standard deviation of the
   * noise process
   * \param[in] m the mean function \f$m\f$
   * \param[in] k the covariance function \f$k\f$
   */
  GPI(const XType& X, const YType& y, Scalar sigma, Scalar tau,
      const MeanFunctionType& m, const CovarianceFunctionType& k)
      : GPI(X, y, Matrix
            <Eigen::Matrix
             <typename YType::scalar_type, YType::RowsAtCompileTime,
              YType::RowsAtCompileTime> >(
                Eigen::Matrix
                <typename YType::scalar_type, YType::RowsAtCompileTime,
                 YType::RowsAtCompileTime>::Identity(y.rows(), y.rows())),
            sigma, tau, 1, m, k) {}
  ///@}

  /** Minus log of the marginal likelihood of the mean observations given the
   * mean and covariance functions, and their Scalar parameters.
    \f[-\log p(\mathbf{y}|\mathbf{X},\mathbf{S},n_r,m,k)
    = \frac{1}{2} \mathbf{\epsilon}^\top \mathbf{\Omega}^{-1} \mathbf{\epsilon}
    + \frac{1}{2} \log |\mathbf{\Omega}| + \frac{M}{2}\log(2\pi) \f]
    \f[\mathbf{\epsilon} \equiv \mathbf{y} - \mathbf{m}(\mathbf{X}) \quad
    \mathbf{\Omega} \equiv \frac{\sigma^2}{N} \mathbf{S}
    +\mathbf{K}(\mathbf{X},\mathbf{X})\f]
    */
  auto minus_log_likelihood() const -> decltype(mvn_lik_.value()) {
    // return the value of the mvn
    return mvn_lik_.value();
  }

  /** \name Posterior mean and covariance functions
   * \f$X^\star,{X'}^\star \in \mathcal{M}_{N,d}(\mathbb{K})\f$ for any
   * \f$N\f$ the user may want.
   */

  ///@{
  /** Posterior mean function
  * \f[\mathbb{E}[f](\mathbf{X}^\star) = \mathbf{m}(\mathbf{X}^\star)
  * + \mathbf{K}(\mathbf{X},\mathbf{X}^\star)^\top \mathbf{\Omega}^{-1}
  * \mathbf{\epsilon}
  * \f]
  */
  template <class XStarType>
  auto posterior_mean(XStarType XStar)
      const -> decltype(VectorXd::Apply(mean_, XStar)
                        + MatrixXd::Apply(cov_, X_, XStar).transpose()
                          * Omega_.decomposition().solve(y_ - mx_)) {
    // build m(x*)
    auto mxs = VectorXd::Apply(mean_, XStar);
    // build k(X,x*)
    auto kxx = MatrixXd::Apply(cov_, X_, XStar);
    // return m + k^T * Omega^{-1} * epsilon
    return mxs + kxx.transpose() * Omega_.decomposition().solve(y_ - mx_);
  }

  /** Posterior covariance function
   * \f[
   * \mathbb{V}[f](\mathbf{X}^\star,\mathbf{X'}^\star) =
   *    k(\mathbf{X}^\star,\mathbf{X'}^\star)
   *     - \mathbf{K}(\mathbf{X},\mathbf{X}^\star)^\top \mathbf{\Omega}^{-1}
   *       \mathbf{K}(\mathbf{X},\mathbf{X'}^\star)
   * \f]
   */
  template <class XStarType, class XStarPrimeType>
  auto posterior_covariance(XStarType XStar, XStarPrimeType XStarPrime)
      const -> decltype(MatrixXd::Apply(cov_, XStar, XStarPrime)
                        - MatrixXd::Apply(cov_, X_, XStar).transpose()
                          * Omega_.decomposition().solve(
                                MatrixXd::Apply(cov_, X_, XStarPrime))) {
    // build k(x*,x'*)
    auto k1 = MatrixXd::Apply(cov_, XStar, XStarPrime);
    // build k(X,x*)
    auto k2 = MatrixXd::Apply(cov_, X_, XStar);
    // build k(X,x'*)
    auto k3 = MatrixXd::Apply(cov_, X_, XStarPrime);
    // return
    return k1 - k2.transpose() * Omega_.decomposition().solve(k3);
  }
  ///@}
};

template <class X, class Y, class S, class M, class C>
GPI<X, Y, S, M, C> make_gpi(const X& x, const Y& y, const S& s, Scalar sigma,
                            Scalar tau, unsigned nr, const M& m, const C& c) {
  return GPI<X, Y, S, M, C>(x, y, s, sigma, tau, nr, m, c);
}
template <class X, class Y, class M, class C>
auto make_gpi(const X& x, const Y& y, Scalar sigma, Scalar tau, const M& m,
              const C& c)
    -> GPI
    <X, Y, Matrix
     <Eigen::Matrix
      <typename Y::scalar_type, Y::RowsAtCompileTime, Y::RowsAtCompileTime> >,
     M, C> {
  return GPI
      <X, Y, Matrix
       <Eigen::Matrix
        <typename Y::scalar_type, Y::RowsAtCompileTime, Y::RowsAtCompileTime> >,
       M, C>(x, y, sigma, tau, m, c);
}
}
#endif /* GPI_H */
