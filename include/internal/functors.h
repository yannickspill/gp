#ifndef FUNCTORS_H
#define FUNCTORS_H

#include <type_traits>
#include <Eigen/Dense>

// defines some operators
// we don't use std::plus and alike as Lhs and Rhs can have different types
//
// the alt versions of the operators are just there to provide two different
// types for the same operation. This allows us to define different traits for
// objects that derive from MatrixBase or for objects that derive from
// ScalarBase by passing different operators to it. The convention is:
// MatrixBase objects use the binary_op while ScalarBase use alt_binary_op.
// exception: quotient, since there is no matrix quotient.

namespace GP {
namespace internal {

struct sum_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a + b;
  }
};
struct alt_sum_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a + b;
  }
};

struct difference_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a - b;
  }
};

struct alt_difference_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a - b;
  }
};

struct product_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a * b;
  }
};

struct alt_product_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a * b;
  }
};

struct quotient_binary_op {
  template <class ResultType, class Lhs, class Rhs>
  const ResultType operator()(const Lhs& a, const Rhs& b) const {
    return a / b;
  }
};
}
}

#endif /* FUNCTORS_H */
