#ifndef INTERNAL_DECOMPOSITION_H
#define INTERNAL_DECOMPOSITION_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived, template <typename> class Policy>
    struct traits<Decomposition<Derived, Policy> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename traits<Policy<Derived> >::result_type result_type;
};

//cholesky Decomposition decomposition. Uses lower part by default, see Eigen doc.
template <typename Derived, template <typename> class Policy = LDLTPolicy>
class Decomposition : public MatrixBase<Decomposition<Derived> >,
                      public Policy<Derived> {
 public:
  typedef typename traits<Decomposition<Derived> >::scalar_type scalar_type;
  typedef typename traits<Decomposition<Derived> >::result_type result_type;

 private:
  const Derived& data_;

 public:
  // constructor
  Decomposition(const Derived& data) : data_(data) {}

  // actual computation
  const result_type& get() const { return this->do_decomp(data_); }

  unsigned get_version() const {
    return data_.get_version();
  }
};
}
}
#endif /* INTERNAL_DECOMPOSITION_H */
