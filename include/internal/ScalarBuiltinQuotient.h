#ifndef SCALAR_BUILTIN_QUOTIENT_H
#define SCALAR_BUILTIN_QUOTIENT_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarBuiltinQuotient
template <class ScalarExpression>
struct traits<ScalarBuiltinQuotient<ScalarExpression> > {
  typedef typename ScalarExpression::scalar_type scalar_type;
  typedef typename ScalarExpression::result_type result_type;
};

// expression template for product of a Scalar with anything convertible to a
// double
template <class ScalarExpression>
class ScalarBuiltinQuotient : public ScalarBase
                             <ScalarBuiltinQuotient<ScalarExpression> > {
 private:
  const ScalarExpression& lhs_;
  double rhs_;

 public:
  typedef typename traits
      <ScalarBuiltinQuotient<ScalarExpression> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarBuiltinQuotient<ScalarExpression> >::result_type result_type;

 public:
  // constructor
  ScalarBuiltinQuotient(const ScalarExpression& lhs, double rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() / rhs_; }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* SCALAR_BUILTIN_QUOTIENT_H */
