#ifndef INTERNAL_MATRIX_BASE_H
#define INTERNAL_MATRIX_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"

namespace GP {
namespace internal {

//! Base class for any matrix
template <class Derived> class MatrixBase : public GPBase<Derived> {

 protected:
  MatrixBase() {}  // only children can instantiate it

 public:
  // typedefs
  typedef typename traits<Derived>::scalar_type scalar_type;
  typedef typename traits<Derived>::result_type result_type;

 public:
  // allow implicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }

  // transposition
  Transpose<Derived> transpose() const {
    return Transpose<Derived>(asDerived());
  }

  // decompose matrix, use Cholesky LDLT decomposition by default
  template <template <class> class Policy = LDLTPolicy>
  Decomposition<Derived, Policy> decomposition() const {
    return Decomposition<Derived, Policy>(asDerived());
  }

  // log-determinant, can only be applied to LDLT decomposition for now
  // write another specialization in LogDeterminant.h if you need more
  LogDeterminant<Derived> logdet() const {
    return LogDeterminant<Derived>(asDerived());
  }

  // solve AX=B system, and yield X
  // for now, only implemented on Decomposition child classes
  template <class OtherDerived>
  Solve<Derived, OtherDerived> solve(const OtherDerived& B) const {
    return Solve<Derived, OtherDerived>(asDerived(), B);
  }
};

// sum
template <class Lhs, class Rhs>
const MatrixSum<Lhs, Rhs> operator+(const MatrixBase<Lhs>& lhs,
                                    const MatrixBase<Rhs>& rhs) {
  return MatrixSum<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}

// difference
template <class Lhs, class Rhs>
const MatrixDifference<Lhs, Rhs> operator-(const MatrixBase<Lhs>& lhs,
                                           const MatrixBase<Rhs>& rhs) {
  return MatrixDifference<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}

// products
//    Matrix Matrix
template <class Lhs, class Rhs>
const MatrixMatrixProduct<Lhs, Rhs> operator*(const MatrixBase<Lhs>& lhs,
                                              const MatrixBase<Rhs>& rhs) {
  return MatrixMatrixProduct<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//   double Matrix
template <class MatrixExpression>
const MatrixBuiltinProduct<MatrixExpression> operator*(
    double lhs, const MatrixBase<MatrixExpression>& rhs) {
  return MatrixBuiltinProduct<MatrixExpression>(lhs, rhs.asDerived());
}
//   Matrix double
template <class MatrixExpression>
const MatrixBuiltinProduct<MatrixExpression> operator*(const MatrixBase
                                                       <MatrixExpression>& lhs,
                                                       double rhs) {
  return MatrixBuiltinProduct<MatrixExpression>(rhs, lhs.asDerived());
}

//    Matrix / builtin
template <class Lhs>
const MatrixBuiltinQuotient<Lhs> operator/(const MatrixBase<Lhs>& lhs,
                                           double rhs) {
  return MatrixBuiltinQuotient<Lhs>(lhs.asDerived(), rhs);
}
}
}
#endif /* INTERNAL_MATRIX_BASE_H */
