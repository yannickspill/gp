#ifndef OPERATIONS_BASE_H
#define OPERATIONS_BASE_H

#include "ForwardDeclarations.h"

#include <Eigen/Core>

namespace GP {
namespace internal {
namespace op {
namespace base {

// check whether scalar types are compatible
template <class Lhs, class Rhs> struct CheckScalarTypesAreCompatible {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
};

// define scalar and input types (unary case)
template <class GPDerived> struct SetScalarAndInput {
  // scalar_type
  typedef typename GPDerived::scalar_type scalar_type;
  // input_type
  typedef typename GPDerived::result_type input_type;
};

// define scalar and input types (binary case)
template <class Lhs, class Rhs>
struct SetScalarAndInputs : CheckScalarTypesAreCompatible<Lhs, Rhs> {
  // scalar_type
  typedef typename Lhs::scalar_type scalar_type;
  // input_types
  typedef typename Lhs::result_type lhs_type;
  typedef typename Rhs::result_type rhs_type;
};

// check whether objects have same shape
template <class Lhs, class Rhs> struct CheckHaveSameShape {
  static_assert(Lhs::RowsAtCompileTime == Eigen::Dynamic
                || Rhs::RowsAtCompileTime == Eigen::Dynamic
                || Lhs::RowsAtCompileTime == Rhs::RowsAtCompileTime,
                "Objects must have same number of rows!");
  static_assert(Lhs::ColsAtCompileTime == Eigen::Dynamic
                || Rhs::ColsAtCompileTime == Eigen::Dynamic
                || Lhs::ColsAtCompileTime == Rhs::ColsAtCompileTime,
                "Objects must have same number of columns!");
};

// resulting shape is preserved (unary case)
template <class GPDerived>
struct SetShapeFromInput : SetScalarAndInput<GPDerived> {
  // rows and columns
  enum {
    RowsAtCompileTime = GPDerived::RowsAtCompileTime,
    ColsAtCompileTime = GPDerived::ColsAtCompileTime
  };
};

// resulting shape is preserved (binary case)
template <class Lhs, class Rhs>
struct SetShapeFromInputs : CheckScalarTypesAreCompatible<Lhs, Rhs>,
                            CheckHaveSameShape<Lhs, Rhs>,
                            SetScalarAndInputs<Lhs, Rhs> {
  // rows and columns
  enum {
    RowsAtCompileTime = Lhs::RowsAtCompileTime,
    ColsAtCompileTime = Rhs::ColsAtCompileTime
  };
};

// SetParent::Parent is the type of the parent of GPDerived
template <class GPDerived> struct SetParent {
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

// SetCommonParent: SetParent for binary operators
template <class Lhs, class Rhs> struct SetCommonParent {
  template <class Derived>
  using Parent = typename std::conditional<
      // if
      std::is_same<typename SetParent<Lhs>::template Parent<Derived>,
                   typename SetParent<Rhs>::template Parent<Derived> >::value,
      // then
      typename SetParent<Lhs>::template Parent<Derived>,
      // else
      GPBase<Derived> >::type;
};

// Matrix -> Scalar
template <class GPDerived>
struct ScalarOperation : SetScalarAndInput<GPDerived> {
  // shape is 1x1
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
  // Parent class is always ScalarBase
  template <class OtherDerived> using Parent = ScalarBase<OtherDerived>;
};

// Matrix -> Matrix with possible change in shape
// unary case
template <class GPDerived>
struct MatrixOperation : SetScalarAndInput<GPDerived> {
  template <class OtherDerived> using Parent = MatrixBase<OtherDerived>;
};
//binary case
/* not needed for now
template <class Lhs, class Rhs>
struct MatrixOperation : SetScalarAndInputs<Lhs, Rhs> {
  template <class OtherDerived> using Parent = MatrixBase<OtherDerived>;
};
*/

// define basic typedefs for unary operations that don't change the shape of the
// resulting object
template <class GPDerived>
struct UnaryCoeffWiseOperation : SetShapeFromInput<GPDerived>,
                                 SetParent<GPDerived> {};

// define basic typedefs for binary operations that don't change the shape of
// the resulting object
template <class Lhs, class Rhs>
struct BinaryCoeffWiseOperation : SetShapeFromInputs<Lhs, Rhs>,
                                  SetCommonParent<Lhs, Rhs> {};
}  // base
}  // op
}  // internal
}  // GP
#endif /* OPERATIONS_BASE_H */
