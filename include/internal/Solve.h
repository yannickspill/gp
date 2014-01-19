#ifndef SOLVE_H
#define SOLVE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

// traits
template <class Derived, class OtherDerived>
struct traits<Solve<Derived, OtherDerived> > {
  static_assert(std::is_same<typename Derived::scalar_type,
                             typename OtherDerived::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Derived::scalar_type scalar_type;
  typedef Eigen::internal::solve_retval
      <typename Derived::result_type, typename OtherDerived::result_type>
          result_type;
};

// solve AX = B for X, given the decomposition for A
template <class Derived, class OtherDerived>
class Solve : public MatrixBase<Solve<Derived, OtherDerived> > {
 public:
  typedef typename traits<Solve>::scalar_type scalar_type;
  typedef typename traits<Solve>::result_type result_type;

 private:
  Derived decomp_;
  OtherDerived mat_;

 public:
  // constructor
  Solve(const Derived& decomp, const OtherDerived& mat)
      : decomp_(decomp), mat_(mat) {}

  // actual computation
  result_type get() const { return decomp_.get().solve(mat_.get()); }

  unsigned get_version() const {
    return decomp_.get_version() + mat_.get_version();
  }
};
}
}
#endif /* SOLVE_H */
