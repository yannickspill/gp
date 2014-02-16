#ifndef INTERNAL_MATRIX_BASE_H
#define INTERNAL_MATRIX_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"
#include "Matrix.h"

#include <functional>

namespace GP {
namespace internal {

//! Base class for any matrix
template <class Derived> class MatrixBase : public GPBase<Derived> {

 protected:
  MatrixBase() {}  // only children can instantiate it

 public:
  // allow implicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }

  // rows and columns
  unsigned rows() const { return asDerived().get().rows(); }
  unsigned cols() const { return asDerived().get().cols(); }

  // trace
  UnaryOp<op::Trace, Derived> trace() const {
    return UnaryOp<op::Trace, Derived>(asDerived());
  }

  // transposition
  UnaryOp<op::Transpose, Derived> transpose() const {
    return UnaryOp<op::Transpose, Derived>(asDerived());
  }

  // decompose matrix, use Cholesky LDLT decomposition by default
  template <template <class> class Policy = LDLTPolicy>
  Decomposition<Derived, Policy> decomposition() const {
    return Decomposition<Derived, Policy>(asDerived());
  }

  //! build diagonal matrix from vector
  UnaryOp<op::DiagonalMatrixFromVector, Derived> asDiagonal() {
      return UnaryOp<op::DiagonalMatrixFromVector, Derived>(asDerived());
  }
  
  //! Yield a matrix by applying a univariate function to every row of an input
  //matrix. Will not check whether func is compatible with the rows of the
  //matrix. Will only compile if the function returns a row vector
  template <class Functor, class InMat>
  static MatrixFromUnivariateFunctor<Functor, InMat>
  Apply(const Functor& func, const InMat& mat) {
    return MatrixFromUnivariateFunctor<Functor, InMat>(func, mat);
  }

  //! Yield a matrix by applying a bivariate function to every row of two input
  //matrices. Will not check whether func is compatible with the rows of the
  //matrix. Will only compile if the function returns a scalar, or 1x1 matrix.
  template <class Functor, class InMat1, class InMat2>
  static MatrixFromBivariateFunctor<Functor, InMat1, InMat2>
  Apply(const Functor& func, const InMat1& mat1, const InMat2& mat2) {
    return MatrixFromBivariateFunctor
        <Functor, InMat1, InMat2>(func, mat1, mat2);
  }

  //! Yield a matrix by applying a bivariate function to every pair of rows of
  // an input matrix.
  // will not check whether func is compatible with the rows of the matrix
  // will only compile if the function returns a double or a 1x1 matrix
  template <class Functor, class InMat>
  static SymmetricMatrixFromBivariateFunctor<Functor, InMat>
  SymmetricApply(const Functor& func, const InMat& mat) {
    return SymmetricMatrixFromBivariateFunctor<Functor, InMat>(func, mat);
  }

  //! Broadcast a constant Scalar expression to a matrix/vector
  //\see declaration in
  template <class ScalarExpr>
  static MatrixFromScalar<ScalarExpr, Derived>
  Broadcast(const ScalarBase<ScalarExpr>& scal, unsigned nrows,
            unsigned ncols) {
    return MatrixFromScalar
        <ScalarExpr, Derived>(scal.asDerived(), nrows, ncols);
  }
};

// sums
//  Matrix Matrix
template <class Lhs, class Rhs>
const BinaryOp<op::Sum, Lhs, Rhs> operator+(const MatrixBase<Lhs>& lhs,
                                    const MatrixBase<Rhs>& rhs) {
  return BinaryOp<op::Sum, Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//  double Matrix
template <class MatrixExpression>
const BinaryOp<op::Sum, double, MatrixExpression> operator+(
    double lhs, const MatrixBase<MatrixExpression>& rhs) {
  return BinaryOp<op::Sum, double, MatrixExpression>(lhs, rhs.asDerived());
}
//  Matrix double
template <class MatrixExpression>
const BinaryOp<op::Sum, MatrixExpression, double> operator+(
    const MatrixBase<MatrixExpression>& lhs, double rhs) {
  return BinaryOp<op::Sum, MatrixExpression, double>(lhs.asDerived(), rhs);
}

// differences
//  Matrix Matrix
template <class Lhs, class Rhs>
const MatrixDifference<Lhs, Rhs> operator-(const MatrixBase<Lhs>& lhs,
                                           const MatrixBase<Rhs>& rhs) {
  return MatrixDifference<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//  double Matrix
template <class MatrixExpression>
const MatrixBuiltinDifference<MatrixExpression> operator-(
    const MatrixBase<MatrixExpression>& lhs, double rhs) {
  return MatrixBuiltinDifference<MatrixExpression>(lhs.asDerived(), rhs);
}
//  Matrix double
template <class MatrixExpression>
const BuiltinMatrixDifference<MatrixExpression> operator-(
    double lhs, const MatrixBase<MatrixExpression>& rhs) {
  return BuiltinMatrixDifference<MatrixExpression>(lhs, rhs.asDerived());
}
// Matrix opposite
template <class MatrixExpression>
const UnaryOp<op::Opposite, MatrixExpression> operator-(const MatrixBase
                                                 <MatrixExpression>& rhs) {
  return UnaryOp<op::Opposite, MatrixExpression>(rhs.asDerived());
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
