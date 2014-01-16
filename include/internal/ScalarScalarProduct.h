#ifndef SCALAR_SCALAR_PRODUCT_H
#define SCALAR_SCALAR_PRODUCT_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarScalarProduct
template <class Lhs, class Rhs> struct traits<ScalarScalarProduct<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
  typedef branch_tag node_type;
};

// expression template for products involving only Scalars
template <typename Lhs, typename Rhs>
class ScalarScalarProduct : public ScalarBase<ScalarScalarProduct<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef typename traits
      <ScalarScalarProduct<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits
      <ScalarScalarProduct<Lhs, Rhs> >::result_type result_type;
  typedef typename traits<ScalarScalarProduct<Lhs, Rhs> >::node_type node_type;

 public:
  // constructor
  ScalarScalarProduct(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() * rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};

// operator*
template <class Lhs, class Rhs>
const ScalarScalarProduct<Lhs, Rhs> operator*(const ScalarBase<Lhs>& lhs,
                                              const ScalarBase<Rhs>& rhs) {
  return ScalarScalarProduct<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* SCALAR_SCALAR_PRODUCT_H */
