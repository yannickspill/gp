#ifndef INTERNAL_FORWARD_DECLARATIONS_H
#define INTERNAL_FORWARD_DECLARATIONS_H

namespace GP {
namespace internal {
namespace op{
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

template <template <class> class UnaryOperator, class Operand> class UnaryOp;
template <template <class, class> class BinaryOperator, class Lhs, class Rhs>
class BinaryOp;

// base class
template <class Derived> class GPBase;

// scalar class
template <class Derived> class ScalarBase;
class Scalar;
class ConstScalar;
template<class Derived> class ScalarFromMatrix;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// matrix operations
template <class Lhs, class Rhs> class MatrixDifference;
template <class Derived> struct LDLTPolicy;
template <class Derived, template<typename> class Policy> class Decomposition;
template <class Derived> class LogDeterminant;
template <class Derived, class OtherDerived> class Solve;

//cache
template <class Object> class Cache;
template <class Object> class CachePlugins;

//functor related
template <class OutExpr, class... InExprs> class Functor;
template <class Functor, class InMat> class MatrixFromUnivariateFunctor;
template <class Functor, class InMat>
class SymmetricMatrixFromBivariateFunctor;
template <class Functor, class InMat1, class InMat2>
class MatrixFromBivariateFunctor;
template<class ScalarExpr, class Derived> class MatrixFromScalar;
}
}
#endif /*FINTERNAL_ORWARD_DECLARATIONS_H*/
