#ifndef COVARIANCE_FUNCTIONS_H
#define COVARIANCE_FUNCTIONS_H

#include "Scalar.h"
#include "Matrix.h"
#include "Functor.h"
#include "macros.h"

#include <Eigen/Dense>

namespace GP {

//! Squared exponential covariance function
//\f[k(\mathbf{x}, \mathbf{y}; \mathbf{M}) =
//\exp\left( -\frac{1}{2} * (\mathbf{x}-\mathbf{y})^T * \mathbf{M} *
//(\mathbf{x}-\mathbf{y}) \right) \f]
template <class SymMat> class SquaredExponentialCovarianceFunction {

  typedef Matrix
      <Eigen::Matrix<typename SymMat::scalar_type,
                     SymMat::result_type::RowsAtCompileTime, 1> > in_type;

  SymMat M_;
  in_type x_, y_;
  decltype(make_functor((-0.5 * (x_ - y_).transpose() * M_*(x_ - y_)).exp(), x_,
                        y_)) f_;

  void mk_functor() {
    x_ = in_type(M_.rows());
    y_ = in_type(M_.rows());
    auto diff = x_ - y_;
    f_ = make_functor((-0.5 * diff.transpose() * M_ * diff).exp(), x_, y_);
  }

 public:
  SquaredExponentialCovarianceFunction(const SymMat& M) : M_(M) {mk_functor();}

  template <class Vec1, class Vec2> double value(Vec1&& v1, Vec2&& v2) const {
    return f_(std::forward<Vec1>(v1), std::forward<Vec2>(v2));
  }
};

template <class SymMat>
SquaredExponentialCovarianceFunction<SymMat> make_se_covariance(SymMat&& M) {
  return SquaredExponentialCovarianceFunction<SymMat>(M);
}

template <unsigned dims = 1>
auto make_se_covariance(Scalar lambda)
    -> SquaredExponentialCovarianceFunction
    <decltype(VectorXd::Broadcast(1. / (lambda* lambda), dims, 1)
                  .asDiagonal())> {
  auto M = VectorXd::Broadcast(1. / (lambda * lambda), dims, 1).asDiagonal();
  return SquaredExponentialCovarianceFunction<decltype(M)>(M);
}
}
#endif /* COVARIANCE_FUNCTIONS_H */
