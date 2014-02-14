#ifndef INTERNAL_BUILTINS_SCALAR_DIFFERENCE_H
#define INTERNAL_BUILTINS_SCALAR_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// expression template for Scalar - Builtin
template <typename Rhs>
class BuiltinScalarDifference : public ScalarBase
                                <BuiltinScalarDifference<Rhs> > {
 private:
  double lhs_;
  Rhs rhs_;

 public:
  static_assert(std::is_convertible<typename Rhs::scalar_type, double>::value,
                "cannot mix different scalar types");
  typedef double scalar_type;
  typedef double result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 public:
  // constructor
  BuiltinScalarDifference(double lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_ - rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_BUILTINS_SCALAR_DIFFERENCE_H */
