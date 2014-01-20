#ifndef BINARY_OP_H
#define BINARY_OP_H

#include "macros.h"
#include "operator_traits.h"
#include "internal/GPBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

namespace {
//Parent<Lhs, Rhs, Derived> will be
//  MatrixBase<Derived> if both Lhs and Rhs inherit from it
//  ScalarBase<Derived> if both Lhs and Rhs do not inherit from MatrixBase
//  GPBase<Derived> in the mixed case
template <class Lhs, class Rhs, class Derived>
using Parent = typename std::conditional<
        std::is_convertible<Lhs, MatrixBase<Lhs> >::value,
        typename std::conditional<
            std::is_convertible<Rhs, MatrixBase<Rhs> >::value,
            MatrixBase<Derived>,
            GPBase<Derived>
           >::type,
        typename std::conditional<
            std::is_convertible<Rhs, MatrixBase<Rhs> >::value,
            GPBase<Derived>,
            ScalarBase<Derived>
           >::type
       >::type;
}

template <typename Lhs, typename Rhs, class Op>
class BinaryOp : public Parent<Lhs, Rhs, BinaryOp<Lhs, Rhs, Op> > {
 public:
  typedef typename traits<BinaryOp<Lhs, Rhs, Op> >::scalar_type scalar_type;
  typedef typename traits<BinaryOp<Lhs, Rhs, Op> >::result_type result_type;

 private:
  const Lhs& lhs_;
  const Rhs& rhs_;
  const Op op_;

 public:
  // constructor
  BinaryOp(const Lhs& lhs, const Rhs& rhs, const Op& op = Op())
      : lhs_(lhs), rhs_(rhs), op_(op) {}

  // actual computation
  result_type get() const {
    return op_.template operator()<result_type>(lhs_.get(), rhs_.get());
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* BINARY_OP_H */
