#ifndef OPERATIONS_BASE_H
#define OPERATIONS_BASE_H

#include "ForwardDeclarations.h"

namespace GP {
namespace internal {
namespace op {
namespace base {

// define scalar and input types
template <class GPDerived> struct Basics {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // input_type
  typedef typename GPDerived::result_type input_type;
};

// common typedefs for all operations
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

// ParentGetter for binary operators
template <class Lhs, class Rhs> struct ParentGetter {
  template <class Derived>
  using Parent = typename std::conditional<
      // if
      std::is_same<ParentGetter<Lhs>, ParentGetter<Rhs> >::value,
      // then
      ParentGetter<Lhs>::template Parent<Derived>,
      // else
      GPBase<Derived> >::type;
};

// Matrix -> Scalar
template <class GPDerived>
struct ScalarOperation : AttribsFromInput<GPDerived> {
  // Parent class is always ScalarBase
  template <class OtherDerived> using Parent = ScalarBase<OtherDerived>;
};

// Matrix -> Matrix with possible change in shape
template <class GPDerived>
struct MatrixOperation : AttribsFromInput<GPDerived> {
  // Parent class is always MatrixBase
  template <class OtherDerived> using Parent = MatrixBase<OtherDerived>;
};

// check whether scalar types are compatible
template <class Lhs, class Rhs> struct ScalarTypesAreCompatible {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
};

// check whether matrices have same shape
template <class Lhs, class Rhs> struct MatricesHaveSameShape {
  static_assert(Lhs::RowsAtCompileTime == Eigen::Dynamic
                || Rhs::RowsAtCompileTime == Eigen::Dynamic
                || Lhs::RowsAtCompileTime == Rhs::RowsAtCompileTime,
                "Matrices must have same number of rows!");
  static_assert(Lhs::ColsAtCompileTime == Eigen::Dynamic
                || Rhs::ColsAtCompileTime == Eigen::Dynamic
                || Lhs::ColsAtCompileTime == Rhs::ColsAtCompileTime,
                "Matrices must have same number of columns!");
};

// define basic typedefs for unary operations that don't change the shape of the
// resulting object
template <class GPDerived>
struct UnaryCoeffWiseOperation : AttribsFromInput<GPDerived>,
                                 ParentGetter<GPDerived> {};

// define basic typedefs for binary operations that don't change the shape of
// the resulting object
template <class Lhs, class Rhs>
struct BinaryCoeffWiseOperation : ScalarTypesAreCompatible<Lhs, Rhs>,
                                  AttribsFromInput<Lhs>,
                                  ParentGetter<Lhs, Rhs> {};

}  // base
}  // op
}  // internal
}  // GP
#endif /* OPERATIONS_BASE_H */
