#ifndef BINARY_OP_H
#define BINARY_OP_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, sum_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  //typedef Eigen::MatrixXd result_type;
  typedef typename Eigen::CwiseBinaryOp
      <Eigen::internal::scalar_sum_op<scalar_type>,
       const typename Lhs::result_type, const typename Rhs::result_type>
          result_type;
};

template <typename Lhs, typename Rhs, class Op>
class BinaryOp : public MatrixBase<BinaryOp<Lhs, Rhs, Op> > {
 public:
  typedef typename traits
      <BinaryOp<Lhs, Rhs, Op> >::scalar_type scalar_type;
  typedef typename traits
      <BinaryOp<Lhs, Rhs, Op> >::result_type result_type;

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
