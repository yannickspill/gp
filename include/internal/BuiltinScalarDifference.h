#ifndef BUILTINS_SCALAR_DIFFERENCE_H
#define BUILTINS_SCALAR_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for BuiltinScalarDifference
template <class Rhs> struct traits<BuiltinScalarDifference<Rhs> > {
  static_assert(std::is_convertible<typename Rhs::scalar_type, double>::value,
                "cannot mix different scalar types");
  typedef double scalar_type;
  typedef double result_type;
};

// expression template for Scalar - Builtin
template <typename Rhs>
class BuiltinScalarDifference : public ScalarBase
                                <BuiltinScalarDifference<Rhs> > {
 private:
  double lhs_;
  const Rhs& rhs_;

 public:
  typedef typename traits
      <BuiltinScalarDifference<Rhs> >::scalar_type scalar_type;
  typedef typename traits
      <BuiltinScalarDifference<Rhs> >::result_type result_type;

 public:
  // constructor
  BuiltinScalarDifference(double lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_ - rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* BUILTINS_SCALAR_DIFFERENCE_H */
