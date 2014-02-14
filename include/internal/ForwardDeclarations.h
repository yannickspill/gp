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

//scalar scalar ops
template <class Lhs, class Rhs> class ScalarScalarSum;
template <class Lhs, class Rhs> class ScalarScalarDifference;
template <class Lhs, class Rhs> class ScalarScalarProduct;
template <class Lhs, class Rhs> class ScalarScalarQuotient;
template <class Lhs> class ScalarOpposite;
template <class Lhs> class ScalarExponential;

//scalar builtin ops
template <class Lhs> class ScalarBuiltinSum;
template <class Lhs> class ScalarBuiltinDifference;
template <class Rhs> class BuiltinScalarDifference;
template <class Rhs> class ScalarBuiltinProduct;
template <class Lhs> class ScalarBuiltinQuotient;
template <class Rhs> class BuiltinScalarQuotient;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

// matrix operations
template <class Lhs, class Rhs> class MatrixSum;
template <class Lhs, class Rhs> class MatrixDifference;
template <class Lhs, class Rhs> class MatrixMatrixProduct;
template <class Lhs> class MatrixOpposite;
template <class Derived> class Trace;
template <class Derived> class Transpose;
template <class Derived> struct LDLTPolicy;
template <class Derived, template<typename> class Policy> class Decomposition;
template <class Derived> class LogDeterminant;
template <class Derived, class OtherDerived> class Solve;
template <class Derived> class DiagonalMatrixFromVector;

//matrix builtin ops
template <class Lhs> class MatrixBuiltinSum;
template <class Lhs> class MatrixBuiltinDifference;
template <class Lhs> class BuiltinMatrixDifference;
template <class Lhs> class MatrixBuiltinProduct;
template <class Lhs> class MatrixBuiltinQuotient;

//matrix scalar ops
template <class Lhs, class Rhs> class MatrixScalarSum;
template <class Lhs, class Rhs> class MatrixScalarDifference;
template <class Lhs, class Rhs> class ScalarMatrixDifference;
template <class Lhs, class Rhs> class MatrixScalarProduct;
template <class Lhs, class Rhs> class MatrixScalarQuotient;

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
