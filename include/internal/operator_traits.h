#ifndef OPERATOR_TRAITS_H
#define OPERATOR_TRAITS_H

#include "ForwardDeclarations.h"

#include <Eigen/Core>

namespace GP {
namespace internal {

// MatrixBase + MatrixBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, sum_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Eigen::CwiseBinaryOp
      <Eigen::internal::scalar_sum_op<scalar_type>,
       const typename Lhs::result_type, const typename Rhs::result_type>
          result_type;
};
// MatrixBase - MatrixBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, difference_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Eigen::CwiseBinaryOp
      <Eigen::internal::scalar_difference_op<scalar_type>,
       const typename Lhs::result_type, const typename Rhs::result_type>
          result_type;
};
// MatrixBase * MatrixBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, product_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Eigen::ProductReturnType
      <typename Lhs::result_type, typename Rhs::result_type>::Type result_type;
};

// ScalarBase + ScalarBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, alt_sum_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};
// ScalarBase - ScalarBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, alt_difference_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};
// ScalarBase * ScalarBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, alt_product_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};
// ScalarBase / ScalarBase
template <class Lhs, class Rhs>
struct traits<BinaryOp<Lhs, Rhs, quotient_binary_op> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef typename Lhs::result_type result_type;
};
}
}

#endif /* OPERATOR_TRAITS_H */
