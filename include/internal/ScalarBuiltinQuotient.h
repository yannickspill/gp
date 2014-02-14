#ifndef INTERNAL_SCALAR_BUILTIN_QUOTIENT_H
#define INTERNAL_SCALAR_BUILTIN_QUOTIENT_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// expression template for product of a Scalar with anything convertible to a
// double
template <class ScalarExpression>
class ScalarBuiltinQuotient : public ScalarBase
                             <ScalarBuiltinQuotient<ScalarExpression> > {
 public:
  // typedefs
  typedef typename ScalarExpression::scalar_type scalar_type;
  typedef typename ScalarExpression::result_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 private:
  ScalarExpression lhs_;
  double rhs_;

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
#endif /* INTERNAL_SCALAR_BUILTIN_QUOTIENT_H */
