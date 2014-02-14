#ifndef INTERNAL_SCALAR_BUILTIN_DIFFERENCE_H
#define INTERNAL_SCALAR_BUILTIN_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// expression template for Scalar - Builtin
template <typename Lhs>
class ScalarBuiltinDifference : public ScalarBase
                                <ScalarBuiltinDifference<Lhs> > {
 public:
  // typedefs
  static_assert(std::is_convertible<typename Lhs::scalar_type, double>::value,
                "cannot mix different scalar types");
  typedef double scalar_type;
  typedef double result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 private:
  Lhs lhs_;
  double rhs_;

 public:
  // constructor
  ScalarBuiltinDifference(const Lhs& lhs, double rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() - rhs_; }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* INTERNAL_SCALAR_BUILTIN_DIFFERENCE_H */
