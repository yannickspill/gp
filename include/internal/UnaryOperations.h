#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H

#include "ForwardDeclarations.h"

#include <utility>

namespace GP {
namespace internal {

//common typedefs for unary operations
template <class GPDerived> struct UnaryCoeffWiseOperation {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // rows and columns
  enum {
    RowsAtCompileTime = GPDerived::RowsAtCompileTime,
    ColsAtCompileTime = GPDerived::ColsAtCompileTime
  };
  // Parent class for resulting expression
  template<class OtherDerived> using Parent = typename std::conditional<
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
  // input_type
  typedef typename GPDerived::result_type input_type;
};

//opposite
template <class GPDerived>
struct Opposite : public UnaryCoeffWiseOperation<GPDerived> {
  typedef decltype(-std::declval
                   <typename Opposite::input_type>()) result_type;
  static result_type apply(const typename Opposite::input_type& b) {
    return -b;
  }
};
}
}
#endif /* UNARY_OPERATIONS_H */
