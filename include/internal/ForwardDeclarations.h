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
template <class Rhs> class ScalarBuiltinProduct;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// matrix operations
template <class Lhs, class Rhs, class op> class BinaryOp;
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
