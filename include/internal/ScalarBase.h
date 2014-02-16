#ifndef INTERNAL_SCALAR_BASE_H
#define INTERNAL_SCALAR_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"
#include "internal/GPBase.h"

namespace GP {
namespace internal {

//! Base class for Scalar (e.g. non-matrix)
template <class Derived> class ScalarBase : public GPBase<Derived> {

 protected:
  ScalarBase() {}  // only children can instantiate it

 public:
  unsigned rows() const { return 1;}
  unsigned cols() const { return 1;}

  // allow implicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }

  UnaryOp<op::ScalarExponential, Derived> exp() const {
      return UnaryOp<op::ScalarExponential, Derived>(asDerived());
  }
};

// sums
//  double Scalar
template <class ScalarExpression>
const BinaryOp<op::Sum, ConstScalar, ScalarExpression> operator+(
    double lhs, const ScalarBase<ScalarExpression>& rhs) {
  return BinaryOp<op::Sum, ConstScalar, ScalarExpression>(lhs, rhs.asDerived());
}
//  Scalar double
template <class ScalarExpression>
const BinaryOp<op::Sum, ScalarExpression, ConstScalar> operator+(
    const ScalarBase<ScalarExpression>& lhs, double rhs) {
  return BinaryOp<op::Sum, ScalarExpression, ConstScalar>(lhs.asDerived(), rhs);
}
// differences
//  double Scalar
template <class ScalarExpression>
const BinaryOp<op::Difference, ConstScalar, ScalarExpression> operator-(
    double lhs, const ScalarBase<ScalarExpression>& rhs) {
  return BinaryOp
      <op::Difference, ConstScalar, ScalarExpression>(lhs, rhs.asDerived());
}
//  Scalar double
template <class ScalarExpression>
const BinaryOp<op::Difference, ScalarExpression, ConstScalar> operator-(
    const ScalarBase<ScalarExpression>& lhs, double rhs) {
  return BinaryOp
      <op::Difference, ScalarExpression, ConstScalar>(lhs.asDerived(), rhs);
}

// Scalar opposite
template <class ScalarExpression>
const UnaryOp<op::Opposite, ScalarExpression> operator-(
    const ScalarBase<ScalarExpression>& rhs) {
  return UnaryOp<op::Opposite, ScalarExpression> (rhs.asDerived());
}

// products
//    Scalar Scalar
template <class Lhs, class Rhs>
const ScalarScalarProduct<Lhs, Rhs> operator*(const ScalarBase<Lhs>& lhs,
                                              const ScalarBase<Rhs>& rhs) {
  return ScalarScalarProduct<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//    double Scalar
template <class ScalarExpression>
const ScalarBuiltinProduct<ScalarExpression> operator*(
    double lhs, const ScalarBase<ScalarExpression>& rhs) {
  return ScalarBuiltinProduct<ScalarExpression>(lhs, rhs.asDerived());
}
//    Scalar double
template <class ScalarExpression>
const ScalarBuiltinProduct<ScalarExpression> operator*(const ScalarBase
                                                       <ScalarExpression>& lhs,
                                                       double rhs) {
  return ScalarBuiltinProduct<ScalarExpression>(rhs, lhs.asDerived());
}
// quotients
//    Scalar Scalar
template <class Lhs, class Rhs>
const ScalarScalarQuotient<Lhs, Rhs> operator/(const ScalarBase<Lhs>& lhs,
                                               const ScalarBase<Rhs>& rhs) {
  return ScalarScalarQuotient<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//    Scalar double
template <class ScalarExpression>
const ScalarBuiltinQuotient<ScalarExpression> operator/(
    const ScalarBase<ScalarExpression>& lhs, double rhs) {
  return ScalarBuiltinQuotient<ScalarExpression>(lhs.asDerived(), rhs);
}
//    double Scalar
template <class ScalarExpression>
const BuiltinScalarQuotient<ScalarExpression> operator/(
    double lhs, const ScalarBase<ScalarExpression>& rhs) {
  return BuiltinScalarQuotient<ScalarExpression>(lhs, rhs.asDerived());
}
}
}
#endif /* INTERNAL_SCALAR_BASE_H */
