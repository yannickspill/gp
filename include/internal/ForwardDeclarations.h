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

//scalar ops
template <class Lhs, class Rhs> class ScalarSum;
template <class Lhs, class Rhs> class ScalarDifference;
template <class Lhs, class Rhs> class ScalarScalarProduct;
template <class Rhs> class ScalarBuiltinProduct;
template <class Rhs> class ScalarScalarQuotient;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// matrix operations
template <class Lhs, class Rhs, class op> class MatrixOp;
template <class Lhs, class Rhs> class MatrixSum;
template <class Lhs, class Rhs> class MatrixDifference;
template <class Lhs, class Rhs> class MatrixMatrixProduct;
template <class Lhs> class MatrixBuiltinProduct;
template <class Lhs, class Rhs> class MatrixScalarQuotient;
template <class Derived> class Transpose;
template <class Derived> struct LDLTPolicy;
template <class Derived, template<typename> class Policy> class Decomposition;
template <class Derived> class LogDeterminant;
template <class Derived, class OtherDerived> class Solve;
}
}
#endif /*FORWARD_DECLARATIONS_H*/
