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
