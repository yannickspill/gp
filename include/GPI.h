#ifndef GPI_H
#define GPI_H

#include "SymmetricMatrixFromFunction.h"
#include "VectorFromFunction.h"
#include "Scalar.h"
#include "ConstEigenObject.h"
#include "MatrixSum.h"
#include "MatrixProduct.h"
#include "MVN.h"

#include <Eigen/Dense>
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
 *                                 \mathbf{K}(\mathbf{X},\mathbf{X}))
 * \f]
 * \f[
       \mathbf{X}^\top \equiv
              \begin{pmatrix} \mathbf{x}_1 \cdots \mathbf{x}_M\end{pmatrix}
 *     \quad \forall i,j \in\llbracket 1,M \rrbracket
 *     \quad \mathbf{m}(\mathbf{X})_i \equiv m(\mathbf{x}_i)
 *     \quad \mathbf{K}(\mathbf{X},\mathbf{X})_{ij}
 *           \equiv k(\mathbf{x}_i,\mathbf{x}_j)
 * \f]
 * \noindent where \f$k\f$ is a covariance function
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
  MeanFunctionType mean_;
  CovarianceFunctionType cov_;
  Scalar sigma_;
  

 public:
  /** Constructor for the Gaussian process
   * \param [in] X \f$\mathbf{X}\f$ A matrix in which each row is a
   * \f$d\f$-dimensional input, corresponding to the abscissa of each of the
   * \f$M\f$ observations.
   * \param [in] y \f$\mathbf{y}\f$ vector of noisy mean observations
   * \param [in] S \f$\mathbf{S}\f$ matrix proportional to the sample variance
   * matrix of these observations
   * \param[in] sigma \f$\sigma\f$ Scalar used as proportionality factor to
   * \f$\mathbf{S}\f$.
   * \param [in] nr \f$n_r\f$ sample size for \f$\mathbf{y}\f$ and
   * \f$\mathbf{S}\f$
   * \param[in] m the mean function \f$m\f$
   * \param[in] k the covariance function \f$k\f$
   */
  GPI(XType X, YType y, SType S, unsigned nr, MeanFunctionType m,
          CovarianceFunctionType k, Scalar sigma)
      : X_(X),
        y_(y),
        S_(S),
        nr_(nr),
        mean_(m),
        cov_(k),
        sigma_(sigma) {
    // build mvn for marginal likelihood
    //  build omega
    
    //
    //
    // build mvn for posterior covariance
  }

  /** Minus log of the marginal likelihood of the mean observations given the
   * mean and covariance functions, and their Scalar parameters.
    \f[-\log p(\mathbf{y}|\mathbf{X},\mathbf{S},n_r,m,k)
    = \frac{1}{2} \mathbf{\epsilon}^\top \mathbf{\Omega}^{-1} \mathbf{\epsilon}
    + \frac{1}{2} \log |\mathbf{\Omega}| + \frac{M}{2}\log(2\pi) \f]
    \f[\mathbf{\epsilon} \equiv \mathbf{y} - \mathbf{m}(\mathbf{X}) \quad
    \mathbf{\Omega} \equiv \frac{\sigma^2}{N} \mathbf{S}
    +\mathbf{K}(\mathbf{X},\mathbf{X})\f]
    */
  double get() const {
    // return the value of the mvn
  }

  //@{
  /** Posterior mean and covariance functions
   * \f[\mathbb{E}[f](\mathbf{x}^\star) = m(\mathbf{x}^\star) 
   * + \mathbf{k}(\mathbf{X},\mathbf{x}^\star)^\top \mathbf{\Omega}^{-1}
   * \mathbf{\epsilon}
   * \f]
   * \f[
   * \mathbb{V}[f](\mathbf{x}^\star,\mathbf{x'}^\star) =
   *    k(\mathbf{x}^\star,\mathbf{x'}^\star)
   *     - \mathbf{k}(\mathbf{X},\mathbf{x}^\star)^\top \mathbf{\Omega}^{-1}
   *       \mathbf{k}(\mathbf{X},\mathbf{x'}^\star)
   * \f]
   * \f[ \mathbf{k}(\mathbf{X},\mathbf{x}^\star)_{i} \equiv
   *        k(\mathbf{x}_i,\mathbf{x}^\star)
   * \f]
   */
  template<class XStarType>
  double get_posterior_mean(XStarType xstar) const {//TODO 
  }

  template<class XStarType, class XStarPrimeType>
  double get_posterior_covariance(XStarType xstar,
          XStarPrimeType xstarprime) const {//TODO
  }
  //@}
};

#endif /* GPI_H */
