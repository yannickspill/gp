#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H

#include "ForwardDeclarations.h"

#include <utility>

namespace GP {
namespace internal {
namespace op {

// common typedefs for unary operations
template <class GPDerived> struct UnaryCoeffWiseOperation {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // input_type
  typedef typename GPDerived::result_type input_type;
  // rows and columns
  enum {
    RowsAtCompileTime = GPDerived::RowsAtCompileTime,
    ColsAtCompileTime = GPDerived::ColsAtCompileTime
  };
  // Parent class for resulting expression
  template <class OtherDerived>
  using Parent = typename std::conditional<
      // if
      std::is_base_of<MatrixBase<GPDerived>, GPDerived>::value,
      // then
      MatrixBase<OtherDerived>,
      // else
      typename std::conditional<
          // if
          std::is_base_of<ScalarBase<GPDerived>, GPDerived>::value,
          // then
          ScalarBase<OtherDerived>,
          // else
          GPBase<OtherDerived> >::type>::type;
};

// operations that take an object and reduce it to a scalar
template <class GPDerived> struct UnaryReductionOperation {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // input type
  typedef typename GPDerived::result_type input_type;
  // rows and columns
  enum {
    RowsAtCompileTime = GPDerived::RowsAtCompileTime,
    ColsAtCompileTime = GPDerived::ColsAtCompileTime
  };
  // Parent class is always ScalarBase
  template <class OtherDerived> using Parent = ScalarBase<OtherDerived>;
};

// opposite
template <class GPDerived>
struct Opposite : public UnaryCoeffWiseOperation<GPDerived> {
  typedef decltype(-std::declval<typename Opposite::input_type>()) result_type;
  static result_type apply(const typename Opposite::input_type& b) {
    return -b;
  }
};

// trace
template <class Derived>
struct Trace : public UnaryReductionOperation<Derived> {
  typedef decltype(std::declval
                   <typename Trace::input_type>().trace()) result_type;
  static result_type apply(const typename Trace::input_type& m) {
    return m.trace();
  }
};
}
}
}
#endif /* UNARY_OPERATIONS_H */
