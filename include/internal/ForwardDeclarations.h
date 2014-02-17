#ifndef INTERNAL_FORWARD_DECLARATIONS_H
#define INTERNAL_FORWARD_DECLARATIONS_H

namespace GP {
namespace internal {

// base classes
template <class Derived> class GPBase;
// scalar
template <class Derived> class ScalarBase;
class Scalar;
class ConstScalar;  // wrapper for double
template <class Derived> class ScalarFromMatrix;  // wrapper for 1x1 matrix
// matrix
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// basic operation expressions
template <template <class> class UnaryOperator, class Operand> class UnaryExpr;
template <template <class, class> class BinaryOperator, class Lhs, class Rhs>
class BinaryExpr;
// operations
namespace op {
template <class Derived> class Opposite;
template <class Derived> class Trace;
template <class Derived> class Transpose;
template <class Derived> class DiagonalMatrixFromVector;
template <class Derived> class ScalarExponential;
template <class Lhs, class Rhs> class Sum;
template <class Lhs, class Rhs> class Difference;
template <class Lhs, class Rhs> class Product;
template <class Lhs, class Rhs> class Quotient;
}

// matrix decomposition related classes
template <class Derived> struct LDLTPolicy;
template <class Derived, template <typename> class Policy> class Decomposition;
template <class Derived> class LogDeterminant;
template <class Derived, class OtherDerived> class Solve;

// caching
template <class Object> class Cache;
template <class Object> class CachePlugins;

// functor related
template <class OutExpr, class... InExprs> class Functor;
template <class Functor, class InMat> class MatrixFromUnivariateFunctor;
template <class Functor, class InMat> class SymmetricMatrixFromBivariateFunctor;
template <class Functor, class InMat1, class InMat2>
class MatrixFromBivariateFunctor;
template <class ScalarExpr, class Derived> class MatrixFromScalar;
}
}
#endif /* INTERNAL_FORWARD_DECLARATIONS_H */
