#ifndef INTERNAL_BUILTIN_SCALAR_QUOTIENT_H
#define INTERNAL_BUILTIN_SCALAR_QUOTIENT_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// expression template for product of a Scalar with anything convertible to a
// double
template <class ScalarExpression>
class BuiltinScalarQuotient : public ScalarBase
                             <BuiltinScalarQuotient<ScalarExpression> > {
 private:
  double lhs_;
  ScalarExpression rhs_;

 public:
  // typedefs
  typedef typename ScalarExpression::scalar_type scalar_type;
  typedef typename ScalarExpression::result_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 public:
  // constructor
  BuiltinScalarQuotient(double lhs, const ScalarExpression& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_ / rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_BUILTIN_SCALAR_QUOTIENT_H */
