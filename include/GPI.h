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
 * data using a gaussian process prior on the function to be interpolated.
 * Inputs are the dataset and the types of the prior mean and covariance
 * functions.
 * \tparam MeanFunctionType the prior mean function type to use.
 * \tparam CovarianceFunctionType prior covariance function type.
 * \tparam InputMatrixType Eigen type of the input matrix
 * \tparam InputVectorType Eigen type of the input vector
 * \tparam d The number of dimensions of the inputs (independent variables).
 *  You can use Eigen::Dynamic if unknown.
 */
template <class MeanFunctionType, class CovarianceFunctionType,
          class InputMatrixType, class InputVectorType,
          unsigned d = Eigen::Dynamic>
class GPI {

  // compile-time checks
  static_assert(d == Eigen::Dynamic || d == MeanFunctionType::input_dimensions,
                "MeanFunctionType has the wrong number of input dimensions");
  static_assert(
      d == Eigen::Dynamic || d == CovarianceFunctionType::input_dimensions,
      "CovarianceFunctionType has the wrong number of input dimensions");
  static_assert(MeanFunctionType::output_dimensions == 1,
                "MeanFunctionType should have an output dimension of 1");
  static_assert(CovarianceFunctionType::output_dimensions == 1,
                "CovarianceFunctionType should have an output dimension of 1");
  static_assert(InputMatrixType::ColsAtCompileTime == Eigen::Dynamic ||
                    InputMatrixType::ColsAtCompileTime == d,
                "InputMatrixType has the wrong number of columns");
  static_assert(InputVectorType::RowsAtCompileTime == Eigen::Dynamic ||
                    InputMatrixType::RowsAtCompileTime == Eigen::Dynamic ||
                    InputMatrixType::RowsAtCompileTime ==
                        InputMatrixType::RowsAtCompileTime,
                "InputVectorType is incompatible with InputMatrixType");

  InputMatrixType X_;
  InputVectorType y_, s_;
  unsigned int n_obs_;
  MeanFunctionType mean_;
  CovarianceFunctionType cov_;
  Scalar sigma_;
  

 public:
  /** Constructor for the gaussian process
   * \param [in] X a \f$M\times d\f$ matrix of coordinates (e.g. independent
   * variables, inputs). Each row is a \f$d\f$-dimensional input,
   * corresponding to the abscissa of each of the \f$M\f$ observations.
   * \param [in] sample_mean \f$I\f$ vector of mean observations (e.g.
   * dependent variables) at each of the previously defined coordinates
   * \param [in] sample_std \f$s\f$ vector of sample standard deviations
   * \param [in] n_obs \f$N\f$ sample size for mean and std
   * \param[in] mf the mean function \f$m\f$ to use
   * \param[in] cf the covariance function \f$w\f$ to use
   * \param[in] sigma used as proportionality factor to S.
   */
  GPI(InputMatrixType X, InputVectorType sample_mean,
      InputVectorType sample_std, unsigned n_obs, MeanFunctionType mf,
      CovarianceFunctionType cf, Scalar sigma)
      : X_(X),
        y_(sample_mean),
        s_(sample_std),
        n_obs_(n_obs),
        mean_(mf),
        cov_(cov),
        sigma_(sigma) {
    // build mvn for marginal likelihood
    //  build omega
    
    //
    //
    // build mvn for posterior covariance
  }

  /** Minus log of the marginal likelihood of the data given the mean
    and covariance functions, and their Scalar parameters.
    \f[-\log p(I|s,N,m,w,X) = \frac{1}{2} \epsilon^\top \Omega^{-1} \epsilon +
    \frac{1}{2} \log |\Omega| + \frac{M}{2}\log(2\pi) \f]
    \f[\epsilon \equiv I - m(X)
       \quad \Omega \equiv \frac{\sigma^2}{N} S +W(X)\f]
    */
  double get() const {
    // return the value of the mvn
  }

  /** Derivative of -log(likelihood) wrt any Scalar */
  double get_derivative(const Scalar& scalar) const {}

  /** Get posterior mean function
   * \f[\hat{I}(x) = m(x) + w(x)^\top \Omega^{-1} \epsilon \f]
   * \f[ w(x) \equiv
   *     \begin{pmatrix} w(x_1,x)\\\vdots\\w(x_M,x) \end{pmatrix}
   *     \quad
   *     m \equiv
   *     \begin{pmatrix} m(x_1)\\\vdots\\m(x_M) \end{pmatrix}
   *     \f]
   */
  double get_posterior_mean(VectorXd x) const;

  /** Posterior covariance
   * \f[\hat{\sigma}^2(x,x') =
   *    w(x,x') - w(x)^\top \Omega^{-1}w(x') \f]
   */
  double get_posterior_covariance(VectorXd x1, VectorXd x2) const;
};

#endif /* GPI_H */
