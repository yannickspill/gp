#ifndef INTERNAL_FORWARD_DECLARATIONS_H
#define INTERNAL_FORWARD_DECLARATIONS_H

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
template <class Lhs, class Rhs> class ScalarScalarSum;
template <class Lhs> class ScalarBuiltinSum;
template <class Lhs, class Rhs> class ScalarScalarDifference;
template <class Lhs> class ScalarBuiltinDifference;
template <class Rhs> class BuiltinScalarDifference;
template <class Lhs, class Rhs> class ScalarScalarProduct;
template <class Rhs> class ScalarBuiltinProduct;
template <class Lhs, class Rhs> class ScalarScalarQuotient;
template <class Lhs> class ScalarBuiltinQuotient;
template <class Rhs> class BuiltinScalarQuotient;

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
template <class Lhs> class MatrixBuiltinQuotient;
template <class Derived> class Transpose;
template <class Derived> struct LDLTPolicy;
template <class Derived, template<typename> class Policy> class Decomposition;
template <class Derived> class LogDeterminant;
template <class Derived, class OtherDerived> class Solve;
}
}
#endif /*FINTERNAL_ORWARD_DECLARATIONS_H*/
