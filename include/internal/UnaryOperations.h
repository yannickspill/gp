#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H

#include "ForwardDeclarations.h"

#include <utility>

namespace GP {
namespace internal {
namespace op {

namespace {
// define scalar and input types
template <class GPDerived> struct Basics {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // input_type
  typedef typename GPDerived::result_type input_type;
};
// common typedefs for unary operations
template <class GPDerived> struct AttribsFromInput : Basics<GPDerived> {
  // rows and columns
  enum {
    RowsAtCompileTime = GPDerived::RowsAtCompileTime,
    ColsAtCompileTime = GPDerived::ColsAtCompileTime
  };
};

// ParentGetter::Parent is the type of the parent of GPDerived
template <class GPDerived> struct ParentGetter {
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

// define basic typedefs for operations that don't change the shape of the
// resulting object
template <class GPDerived>
struct UnaryCoeffWiseOperation : AttribsFromInput<GPDerived>,
                                 ParentGetter<GPDerived> {};

// Matrix -> Scalar
template <class GPDerived>
struct UnaryReductionOperation : AttribsFromInput<GPDerived> {
  // Parent class is always ScalarBase
  template <class OtherDerived> using Parent = ScalarBase<OtherDerived>;
};

// Matrix -> Matrix with possible change in shape
template <class GPDerived>
struct MatrixOperation : AttribsFromInput<GPDerived> {
  // Parent class is always MatrixBase
  template <class OtherDerived> using Parent = MatrixBase<OtherDerived>;
};
}

// opposite
template <class GPDerived>
struct Opposite : UnaryCoeffWiseOperation<GPDerived> {
  typedef decltype(-std::declval<typename Opposite::input_type>()) result_type;
  static result_type apply(const typename Opposite::input_type& b) {
    return -b;
  }
};

// trace
template <class Derived> struct Trace : UnaryReductionOperation<Derived> {
  typedef decltype(std::declval
                   <typename Trace::input_type>().trace()) result_type;
  static result_type apply(const typename Trace::input_type& m) {
    return m.trace();
  }
};

// transpose
template <class Derived> struct Transpose : MatrixOperation<Derived> {
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
struct DiagonalMatrixFromVector : MatrixOperation<Derived> {
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
}
}
}
#endif /* UNARY_OPERATIONS_H */