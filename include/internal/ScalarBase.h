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

  ScalarExponential<Derived> exp() const {
      return ScalarExponential<Derived>(asDerived());
  }
};

// sums
//    Scalar + Scalar
template <class Lhs, class Rhs>
const ScalarScalarSum<Lhs, Rhs> operator+(const ScalarBase<Lhs>& lhs,
                                    const ScalarBase<Rhs>& rhs) {
  return ScalarScalarSum<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//    Scalar + Builtin
template <class Lhs>
const ScalarBuiltinSum<Lhs> operator+(const ScalarBase<Lhs>& lhs, double rhs) {
  return ScalarBuiltinSum<Lhs>(lhs.asDerived(), rhs);
}
//    Builtin + Scalar
template <class Rhs>
const ScalarBuiltinSum<Rhs> operator+(double lhs, const ScalarBase<Rhs>& rhs) {
  return ScalarBuiltinSum<Rhs>(rhs.asDerived(), lhs);
}
// differences
//    Scalar - Scalar
template <class Lhs, class Rhs>
const ScalarScalarDifference<Lhs, Rhs> operator-(const ScalarBase<Lhs>& lhs,
                                           const ScalarBase<Rhs>& rhs) {
  return ScalarScalarDifference<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
//    Scalar - Builtin
template <class Lhs>
const ScalarBuiltinDifference<Lhs> operator-(const ScalarBase<Lhs>& lhs,
                                             double rhs) {
  return ScalarBuiltinDifference<Lhs>(lhs.asDerived(), rhs);
}
//    Builtin - Scalar
template <class Rhs>
const BuiltinScalarDifference<Rhs> operator-(double lhs,
                                           const ScalarBase<Rhs>& rhs) {
  return BuiltinScalarDifference<Rhs>(lhs, rhs.asDerived());
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
