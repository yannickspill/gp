#ifndef BINARY_OPERATIONS_H
#define BINARY_OPERATIONS_H

#include "ForwardDeclarations.h"
#include "OperationsBase.h"

#include <utility>

namespace GP {
namespace internal {
namespace op {

// sum
template <class Lhs, class Rhs>
struct Sum : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Sum::lhs_type>()
                 + std::declval<typename Sum::rhs_type>()) result_type;
  static result_type apply(const typename Sum::lhs_type& lhs,
                           const typename Sum::rhs_type& rhs) {
    return lhs + rhs;
  }
};

}  // op
}  // internal
}  // GP
#endif /* BINARY_OPERATIONS_H */
