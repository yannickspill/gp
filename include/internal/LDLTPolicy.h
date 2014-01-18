#ifndef LDLT_POLICY_H
#define LDLT_POLICY_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<LDLTPolicy<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::LDLT
      <typename Eigen::MatrixBase<typename Derived::result_type>::PlainObject>
          result_type;
};

// cholesky LDLT decomposition policy. Uses lower part by default, see Eigen
// doc.
template <typename Derived> struct LDLTPolicy {

  typedef typename traits<LDLTPolicy<Derived> >::scalar_type scalar_type;
  typedef typename traits<LDLTPolicy<Derived> >::result_type result_type;

  // actual computation
  static result_type decompose(const Derived& data) {
    return data.get().ldlt();
  }

 protected:
  // only derived classes can destruct this policy
  ~LDLTPolicy() {}
};
}
}
#endif /* LDLT_POLICY_H */
