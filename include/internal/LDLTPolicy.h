#ifndef INTERNAL_LDLT_POLICY_H
#define INTERNAL_LDLT_POLICY_H

#include "macros.h"
#include "ForwardDeclarations.h"

#include <Eigen/Core>
#include <Eigen/Cholesky>

namespace GP {
namespace internal {

template <class Derived> class LDLTPolicy {
 public:
  // typedefs
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::LDLT
      <typename Eigen::MatrixBase<typename Derived::result_type>::PlainObject>
          result_type;
 private:
  mutable result_type ldlt_;

 public:
  // initialize ldlt object without calculating anything
  LDLTPolicy() : ldlt_(result_type()) {}

  const result_type& do_decomp(const Derived& data) const {
    ldlt_.compute(data.get());
    return ldlt_;
  }

 protected:
  // prevent non-child classes to destroy this policy
  // see Modern C++ Design (Alexandrescu) p. 13
  ~LDLTPolicy() {};
};
}
}
#endif /* INTERNAL_LDLT_POLICY_H */
