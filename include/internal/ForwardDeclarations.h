#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

namespace GP {
namespace internal {
// declare traits class
template <class Derived> struct traits;

// base class
template <class Derived> class GPBase;
template <class Lhs, class Rhs> class MatrixScalarProduct;

// scalar class
template <class Derived> class ScalarBase;
class Scalar;
template <class Lhs, class Rhs> class ScalarScalarProduct;
template <class Rhs> class ScalarBuiltinProduct;
template <class Rhs> class ScalarBuiltinDivision;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// matrix operations
template <class Lhs, class Rhs> class MatrixSum;
template <class Lhs, class Rhs> class MatrixDifference;
template <class Lhs, class Rhs> class MatrixMatrixProduct;
template <class Lhs> class MatrixBuiltinProduct;
template <class Lhs> class MatrixBuiltinDivision;
template <class Derived> class Transpose;
}
}
#endif /*FORWARD_DECLARATIONS_H*/
