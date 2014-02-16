#ifndef BINARY_OPERATIONS_H
#define BINARY_OPERATIONS_H

#include "ForwardDeclarations.h"
#include "OperationsBase.h"

#include <utility>
#include <type_traits>

namespace GP {
namespace internal {
namespace op {

// Sum
template <class Lhs, class Rhs>
struct Sum : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Sum::lhs_type>() + std::declval
                   <typename Sum::rhs_type>()) result_type;
  static result_type apply(const typename Sum::lhs_type& lhs,
                           const typename Sum::rhs_type& rhs) {
    return lhs + rhs;
  }
};

// Difference
template <class Lhs, class Rhs>
struct Difference : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Difference::lhs_type>() - std::declval
                   <typename Difference::rhs_type>()) result_type;
  static result_type apply(const typename Difference::lhs_type& lhs,
                           const typename Difference::rhs_type& rhs) {
    return lhs - rhs;
  }
};

// Product
template <class Lhs, class Rhs>
struct Product : base::SetScalarAndInputs<Lhs, Rhs>,
                 base::SetCommonParent<Lhs, Rhs> {
  static_assert(Lhs::ColsAtCompileTime == Eigen::Dynamic
                || Rhs::RowsAtCompileTime == Eigen::Dynamic
                || Lhs::ColsAtCompileTime == Rhs::RowsAtCompileTime,
                "Lhs and Rhs have incompatible shapes for a matrix product");
  enum {
    RowsAtCompileTime = Lhs::RowsAtCompileTime,
    ColsAtCompileTime = Rhs::ColsAtCompileTime
  };
  typedef decltype(std::declval<typename Product::lhs_type>() * std::declval
                   <typename Product::rhs_type>()) result_type;
  static result_type apply(const typename Product::lhs_type& lhs,
                           const typename Product::rhs_type& rhs) {
    return lhs * rhs;
  }
};

// Quotient
template <class Lhs, class Rhs>
struct Quotient : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Quotient::lhs_type>() / std::declval
                   <typename Quotient::rhs_type>()) result_type;
  static result_type apply(const typename Quotient::lhs_type& lhs,
                           const typename Quotient::rhs_type& rhs) {
    return lhs / rhs;
  }
};
}  // op
}  // internal
}  // GP
#endif /* BINARY_OPERATIONS_H */
