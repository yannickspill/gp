#ifndef INTERNAL_SOLVE_H
#define INTERNAL SOLVE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

// traits
template <class DerivedMat, template<class> class Policy, class OtherDerived>
struct traits<Solve<Decomposition<DerivedMat, Policy>, OtherDerived> > {
  static_assert(std::is_same<typename DerivedMat::scalar_type,
                             typename OtherDerived::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename DerivedMat::scalar_type scalar_type;
  typedef Eigen::internal::solve_retval
      <typename Decomposition<DerivedMat, Policy>::result_type,
       typename OtherDerived::result_type> result_type;
};

// solve AX = B for X, given the decomposition for A
template <class DerivedMat, template<class> class Policy, class OtherDerived>
class Solve
    <Decomposition<DerivedMat, Policy>,
     OtherDerived> : public MatrixBase
                     <Solve<Decomposition<DerivedMat, Policy>, OtherDerived> > {
 public:
  typedef typename traits<Solve>::scalar_type scalar_type;
  typedef typename traits<Solve>::result_type result_type;

 private:
  const Decomposition<DerivedMat, Policy>& decomp_;
  const OtherDerived& mat_;

 public:
  // constructor
  Solve(const Decomposition<DerivedMat, Policy>& decomp,
        const OtherDerived& mat)
      : decomp_(decomp), mat_(mat) {}

  // actual computation
  result_type get() const { return decomp_.get().solve(mat_.get()); }

  unsigned get_version() const {
    return decomp_.get_version() + mat_.get_version();
  }
};
}
}
#endif /* INTERNAL_SOLVE_H */
