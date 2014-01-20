#ifndef SCALAR_BASE_H
#define SCALAR_BASE_H

#include "macros.h"
#include "functors.h"
#include "internal/ForwardDeclarations.h"
#include "internal/GPBase.h"

namespace GP {
namespace internal {

//! Base class for Scalar (e.g. non-matrix)
template <class Derived> class ScalarBase : public GPBase<Derived> {

 protected:
  ScalarBase() {}  // only children can instantiate it

 public:
  // typedefs
  typedef typename traits<Derived>::scalar_type scalar_type;
  typedef typename traits<Derived>::result_type result_type;

 public:
  // allow implicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }
};

// sum
template <class Lhs, class Rhs>
const BinaryOp<Lhs, Rhs, alt_sum_binary_op> operator+(const ScalarBase
                                                      <Lhs>& lhs,
                                                      const ScalarBase
                                                      <Rhs>& rhs) {
  return BinaryOp
      <Lhs, Rhs, alt_sum_binary_op>(lhs.asDerived(), rhs.asDerived());
}

// difference
template <class Lhs, class Rhs>
const BinaryOp<Lhs, Rhs, alt_difference_binary_op> operator-(const ScalarBase
                                                      <Lhs>& lhs,
                                                      const ScalarBase
                                                      <Rhs>& rhs) {
  return BinaryOp
      <Lhs, Rhs, alt_difference_binary_op>(lhs.asDerived(), rhs.asDerived());
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

// quotient
template <class Lhs, class Rhs>
const BinaryOp<Lhs, Rhs, quotient_binary_op> operator/(const ScalarBase
                                                           <Lhs>& lhs,
                                                           const ScalarBase
                                                           <Rhs>& rhs) {
  return BinaryOp
      <Lhs, Rhs, quotient_binary_op>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* SCALAR_BASE_H */
