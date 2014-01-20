#ifndef INTERNAL_SCALAR_BUILTIN_PRODUCT_H
#define INTERNAL_SCALAR_BUILTIN_PRODUCT_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarBuiltinProduct
template <class ScalarExpression>
struct traits<ScalarBuiltinProduct<ScalarExpression> > {
  typedef typename ScalarExpression::scalar_type scalar_type;
  typedef typename ScalarExpression::result_type result_type;
};

// expression template for product of a Scalar with anything convertible to a
// double
template <class ScalarExpression>
class ScalarBuiltinProduct : public ScalarBase
                             <ScalarBuiltinProduct<ScalarExpression> > {
 private:
  double lhs_;
  const ScalarExpression& rhs_;

 public:
  typedef typename traits
      <ScalarBuiltinProduct<ScalarExpression> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarBuiltinProduct<ScalarExpression> >::result_type result_type;

 public:
  // constructor
  ScalarBuiltinProduct(double lhs, const ScalarExpression& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_ * rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_SCALAR_BUILTIN_PRODUCT_H */
