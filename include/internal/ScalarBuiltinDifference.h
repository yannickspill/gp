#ifndef SCALAR_BUILTIN_DIFFERENCE_H
#define SCALAR_BUILTIN_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarBuiltinDifference
template <class Lhs> struct traits<ScalarBuiltinDifference<Lhs> > {
  static_assert(std::is_convertible<typename Lhs::scalar_type, double>::value,
                "cannot mix different scalar types");
  typedef double scalar_type;
  typedef double result_type;
};

// expression template for Scalar - Builtin
template <typename Lhs>
class ScalarBuiltinDifference : public ScalarBase
                                <ScalarBuiltinDifference<Lhs> > {
 private:
  const Lhs& lhs_;
  double rhs_;

 public:
  typedef typename traits
      <ScalarBuiltinDifference<Lhs> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarBuiltinDifference<Lhs> >::result_type result_type;

 public:
  // constructor
  ScalarBuiltinDifference(const Lhs& lhs, double rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() - rhs_; }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* SCALAR_BUILTIN_DIFFERENCE_H */
