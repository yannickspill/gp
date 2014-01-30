#ifndef INTERNAL_SCALAR_SCALAR_QUOTIENT_H
#define INTERNAL_SCALAR_SCALAR_QUOTIENT_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for ScalarScalarQuotient
template <class Lhs, class Rhs> struct traits<ScalarScalarQuotient<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};

// expression template for products involving only Scalars
template <typename Lhs, typename Rhs>
class ScalarScalarQuotient : public ScalarBase<ScalarScalarQuotient<Lhs, Rhs> > {
 public:
  // typedefs
  typedef typename traits<ScalarScalarQuotient>::scalar_type scalar_type;
  typedef typename traits<ScalarScalarQuotient>::result_type result_type;

 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  // constructor
  ScalarScalarQuotient(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() / rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_SCALAR_SCALAR_QUOTIENT_H */
