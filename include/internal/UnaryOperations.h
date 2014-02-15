#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H

#include "ForwardDeclarations.h"
#include "OperationsBase.h"

#include <utility>
#include <math.h>

namespace GP {
namespace internal {
namespace op {

// opposite
template <class GPDerived>
struct Opposite : base::UnaryCoeffWiseOperation<GPDerived> {
  typedef decltype(-std::declval<typename Opposite::input_type>()) result_type;
  static result_type apply(const typename Opposite::input_type& b) {
    return -b;
  }
};

// trace
template <class Derived> struct Trace : base::ScalarOperation<Derived> {
  typedef decltype(std::declval
                   <typename Trace::input_type>().trace()) result_type;
  static result_type apply(const typename Trace::input_type& m) {
    return m.trace();
  }
};

// transpose
template <class Derived> struct Transpose : base::MatrixOperation<Derived> {
  // rows and columns
  enum {
    ColsAtCompileTime = Derived::RowsAtCompileTime,
    RowsAtCompileTime = Derived::ColsAtCompileTime
  };
  //
  typedef const decltype(std::declval
                         <const typename Transpose::input_type>().transpose())
      result_type;
  static result_type apply(const typename Transpose::input_type& m) {
    return m.transpose();
  }
};

// DiagonalMatrixFromVector
template <class Derived>
struct DiagonalMatrixFromVector : base::MatrixOperation<Derived> {
  // rows and columns
  enum {
    RowsAtCompileTime = Derived::RowsAtCompileTime,
    ColsAtCompileTime = Derived::RowsAtCompileTime
  };
  //
  typedef const decltype(std::declval
                         <const typename DiagonalMatrixFromVector::input_type>()
                             .asDiagonal()) result_type;
  static result_type apply(const typename DiagonalMatrixFromVector::input_type
                           & m) {
    return m.asDiagonal();
  }
};

// Scalar exp()
template <class Derived>
struct ScalarExponential : base::ScalarOperation<Derived> {
  typedef typename ScalarExponential::input_type result_type;
  static result_type apply(const typename ScalarExponential::input_type& in) {
    return exp(in);
  }
};
}  // op
}  // internal
}  // GP
#endif /* UNARY_OPERATIONS_H */
