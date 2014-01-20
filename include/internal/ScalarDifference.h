#ifndef SCALAR_DIFFERENCE_H
#define SCALAR_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarDifference
template <class Lhs, class Rhs> struct traits<ScalarDifference<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};

// expression template for products involving only Scalars
template <typename Lhs, typename Rhs>
class ScalarDifference : public ScalarBase<ScalarDifference<Lhs, Rhs> > {
 private:
  const Lhs& lhs_;
  const Rhs& rhs_;

 public:
  typedef typename traits
      <ScalarDifference<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarDifference<Lhs, Rhs> >::result_type result_type;

 public:
  // constructor
  ScalarDifference(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() - rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* SCALAR_DIFFERENCE_H */
