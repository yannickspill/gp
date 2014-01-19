#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

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

  //transposition
  Transpose<Derived> transpose() const {
      return Transpose<Derived>(asDerived());
  }

  // log-determinant, can only be applied to LDLT decomposition for now
  // write another specialization in Determinant.h if you need more
  LogDeterminant<Derived> logdet() const {
    return LogDeterminant<Derived>(asDerived());
  }

  //Cholesky LDLT decomposition
  Decomposition<Derived> decomposition() const {
      return Decomposition<Derived>(asDerived());
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

// product
template <class Lhs, class Rhs>
const MatrixMatrixProduct<Lhs, Rhs> operator*(const MatrixBase<Lhs>& lhs,
                                              const MatrixBase<Rhs>& rhs) {
  return MatrixMatrixProduct<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* MATRIX_BASE_H */
