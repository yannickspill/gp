#ifndef FUNCTORS_H
#define FUNCTORS_H

#include <type_traits>
#include <Eigen/Dense>

// defines some operators
// we don't use std::plus and alike as Lhs and Rhs can have different types

namespace GP {
namespace internal {

struct sum_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a + b;
  }
};
}
}

#endif /* FUNCTORS_H */
