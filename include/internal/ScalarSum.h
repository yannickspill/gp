#ifndef SCALAR_SUM_H
#define SCALAR_SUM_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarSum
template <class Lhs, class Rhs> struct traits<ScalarSum<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};

// expression template for products involving only Scalars
template <typename Lhs, typename Rhs>
class ScalarSum : public ScalarBase<ScalarSum<Lhs, Rhs> > {
 private:
  const Lhs& lhs_;
  const Rhs& rhs_;

 public:
  typedef typename traits
      <ScalarSum<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarSum<Lhs, Rhs> >::result_type result_type;

 public:
  // constructor
  ScalarSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() + rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* SCALAR_SUM_H */