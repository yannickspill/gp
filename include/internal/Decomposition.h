#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/tags.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived, template <class> class Policy>
struct traits<Decomposition<Derived, Policy > > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Policy<Derived>::result_type result_type;
  typedef branch_tag node_type;
};

// host class for any decomposition type. Uses LDLTPolicy by default.
// accepts any policy that has a decompose() member function
template <class Derived, template <class> class Policy = LDLTPolicy>
class Decomposition : public MatrixBase<Derived>, public Policy<Derived> {
 public:
  typedef typename traits
      <Decomposition<Derived, Policy> >::scalar_type scalar_type;
  typedef typename traits
      <Decomposition<Derived, Policy> >::result_type result_type;
  typedef typename traits
      <Decomposition<Derived, Policy> >::node_type node_type;

 private:
  Derived data_;

 public:
  // constructor
  Decomposition(const Derived& data) : data_(data) {}

  result_type get() const { return this->decompose(data_); }

  unsigned get_version() const { return data_.get_version(); }
};
}
}
#endif /* DECOMPOSITION_H */
