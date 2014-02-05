#ifndef MATRIX_H
#define MATRIX_H

//base classes
#include "internal/ForwardDeclarations.h"
#include "internal/GPBase.h"
#include "internal/MatrixBase.h"

// matrix matrix ops
#include "internal/Matrix.h"
#include "internal/MatrixSum.h"
#include "internal/MatrixDifference.h"
#include "internal/MatrixMatrixProduct.h"
#include "internal/MatrixOpposite.h"
#include "internal/Trace.h"
#include "internal/Transpose.h"
#include "internal/LDLTPolicy.h"
#include "internal/Decomposition.h"
#include "internal/LogDeterminant.h"
#include "internal/Solve.h"
#include "internal/DiagonalMatrixFromVector.h"

// matrix builtin ops
#include "internal/MatrixBuiltinSum.h"
#include "internal/MatrixBuiltinDifference.h"
#include "internal/BuiltinMatrixDifference.h"
#include "internal/MatrixBuiltinProduct.h"
#include "internal/MatrixBuiltinQuotient.h"

// matrix scalar ops
#include "internal/MatrixScalarProduct.h"
#include "internal/MatrixScalarQuotient.h"
#include "internal/MatrixScalarSum.h"
#include "internal/MatrixScalarDifference.h"
#include "internal/ScalarMatrixDifference.h"

// caching
#include "internal/Cache.h"
#include "internal/CachePlugins.h"

//functor related
#include "internal/Functor.h"
#include "internal/MatrixFromUnivariateFunctor.h"
#include "internal/MatrixFromBivariateFunctor.h"
#include "internal/SymmetricMatrixFromBivariateFunctor.h"
#include "internal/MatrixFromScalar.h"

//external libs
#include <Eigen/Dense>

namespace GP {
// promote some internal things to GP namespace
using internal::Matrix;
using internal::Transpose;
using internal::Decomposition;
using internal::LDLTPolicy;
using internal::LogDeterminant;
using internal::Solve;
using internal::MatrixFromUnivariateFunctor;
using internal::MatrixFromBivariateFunctor;
using internal::SymmetricMatrixFromBivariateFunctor;

//convenience typedefs
typedef Matrix<Eigen::RowVectorXd> RowVectorXd;
typedef Matrix<Eigen::VectorXd> VectorXd;
typedef Matrix<Eigen::MatrixXd> MatrixXd;
}

#endif /* MATRIX_H */
