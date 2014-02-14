#ifndef INTERNAL_SOLVE_H
#define INTERNAL SOLVE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace GP {
namespace internal {

//! solve AX = B for X, given the decomposition for A
template <class DecompType, class OtherDerived>
class Solve : public MatrixBase<Solve<DecompType, OtherDerived> > {
 public:
  // typedefs
  static_assert(std::is_same<typename DecompType::scalar_type,
                             typename OtherDerived::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename OtherDerived::scalar_type scalar_type;
  typedef decltype(
      std::declval<typename DecompType::result_type>().solve(
          std::declval<typename OtherDerived::result_type>())) result_type;
  enum {
    RowsAtCompileTime = DecompType::RowsAtCompileTime,
    ColsAtCompileTime = OtherDerived::ColsAtCompileTime
  };
 private:
  DecompType decomp_;
  OtherDerived mat_;

 public:
  // constructor
  Solve(const DecompType& decomp, const OtherDerived& mat)
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
