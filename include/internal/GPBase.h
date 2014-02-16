#ifndef INTERNAL_GP_BASE_H
#define INTERNAL_GP_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"
#include <Eigen/Core>

namespace GP {
namespace internal {

//! Base class for all GP objects
template <class Derived> class GPBase {

 protected:
  GPBase() {}  // only children can instantiate it

 public:
  // allow explicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }

  //caching
  Cache<Derived> cache() const {
      return Cache<Derived>(asDerived());
  }

  //Eigen stuff
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// sums
//  GP GP
template <class Lhs, class Rhs>
const BinaryOp<op::Sum, Lhs, Rhs> operator+(const GPBase<Lhs>& lhs,
                                    const GPBase<Rhs>& rhs) {
  return BinaryOp<op::Sum, Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//  double GP
template <class GPExpression>
const BinaryOp<op::Sum, double, GPExpression> operator+(
    double lhs, const GPBase<GPExpression>& rhs) {
  return BinaryOp<op::Sum, double, GPExpression>(lhs, rhs.asDerived());
}
//  GP double
template <class GPExpression>
const BinaryOp<op::Sum, GPExpression, double> operator+(
    const GPBase<GPExpression>& lhs, double rhs) {
  return BinaryOp<op::Sum, GPExpression, double>(lhs.asDerived(), rhs);
}

//differences
//  Scalar - Matrix (only matrix is 1x1)
template <class Scal, class Mat>
const ScalarMatrixDifference<Scal, Mat> operator-(const ScalarBase<Scal>& lhs,
                                           const MatrixBase<Mat>& rhs) {
  return ScalarMatrixDifference<Scal, Mat>(lhs.asDerived(), rhs.asDerived());
}
//  Matix - Scalar (only matrix is 1x1)
template <class Mat, class Scal>
const MatrixScalarDifference<Mat, Scal> operator-(const MatrixBase<Mat>& lhs,
                                           const ScalarBase<Scal>& rhs) {
  return MatrixScalarDifference<Mat, Scal>(lhs.asDerived(), rhs.asDerived());
}

//products
//  Scalar * Matrix
template <class Scal, class Mat>
const MatrixScalarProduct<Mat, Scal> operator*(const ScalarBase<Scal>& lhs,
                                               const MatrixBase<Mat>& rhs) {
  return MatrixScalarProduct<Mat, Scal>(rhs.asDerived(), lhs.asDerived());
}
//  Matrix * Scalar
template <class Mat, class Scal>
const MatrixScalarProduct<Mat, Scal> operator*(const MatrixBase<Mat>& lhs,
                                               const ScalarBase<Scal>& rhs) {
  return MatrixScalarProduct<Mat, Scal>(lhs.asDerived(), rhs.asDerived());
}

//quotients
// Matrix / Scalar
template <class Mat, class Scal>
const MatrixScalarQuotient<Mat, Scal> operator/(const MatrixBase<Mat>& lhs,
                                               const ScalarBase<Scal>& rhs) {
  return MatrixScalarQuotient<Mat, Scal>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* INTERNAL_GP_BASE_H */
