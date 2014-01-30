#ifndef INTERNAL_DECOMPOSITION_H
#define INTERNAL_DECOMPOSITION_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"

#include <Eigen/Core>

namespace GP {
namespace internal {

template <class Derived, template <typename> class Policy>
    struct traits<Decomposition<Derived, Policy> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename traits<Policy<Derived> >::result_type result_type;
};

//General class for decomposition. Uses LDLT by default.
template <typename Derived, template <typename> class Policy = LDLTPolicy>
class Decomposition : public Policy<Derived>,
                      public GPBase<Decomposition<Derived, Policy> > {
 public:
  // typedefs
  typedef typename traits<Decomposition>::scalar_type scalar_type;
  typedef typename traits<Decomposition>::result_type result_type;

 private:
  Derived data_;

 public:
  // constructor
  Decomposition(const Derived& data) : data_(data) {}

  // actual computation
  const result_type& get() const { return this->do_decomp(data_); }

  // log-determinant, can only be applied to LDLT decomposition for now
  // write another specialization in LogDeterminant.h if you need more
  LogDeterminant<Decomposition> logdet() const {
    return LogDeterminant<Decomposition>(*this);
  }

  // solve AX=B system, and yield X
  template <class OtherDerived>
  Solve<Decomposition, OtherDerived> solve(const OtherDerived& B) const {
    return Solve<Decomposition, OtherDerived>(*this, B);
  }

  unsigned get_version() const {
    return data_.get_version();
  }
};
}
}
#endif /* INTERNAL_DECOMPOSITION_H */
