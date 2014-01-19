#ifndef LDLT_POLICY_H
#define LDLT_POLICY_H

#include "macros.h"
#include "ForwardDeclarations.h"

#include <Eigen/Core>
#include <Eigen/Cholesky>

namespace GP{
namespace internal{

template<class Derived> struct traits<LDLTPolicy<Derived> > {
      typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::LDLT
      <typename Eigen::MatrixBase<typename Derived::result_type>::PlainObject>
          result_type;
};

template<class Derived>
struct LDLTPolicy {
    typedef typename traits<LDLTPolicy<Derived> >::scalar_type scalar_type;
    typedef typename traits<LDLTPolicy<Derived> >::result_type result_type;

    static result_type do_decomp(const Derived& data) {
      return data.get().ldlt();
    }
};

}
}
#endif /* LDLT_POLICY_H */
