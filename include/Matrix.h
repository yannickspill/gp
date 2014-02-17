#ifndef MATRIX_H
#define MATRIX_H

#include "internal/UnaryExpr.h"
#include "internal/UnaryOperations.h"

#include "internal/BinaryExpr.h"
#include "internal/BinaryOperations.h"

#include "internal/ScalarFromMatrix.h"
#include "internal/ConstScalar.h"

//base classes
#include "internal/GPBase.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

// matrix matrix ops
#include "internal/LDLTPolicy.h"
#include "internal/Decomposition.h"
#include "internal/LogDeterminant.h"
#include "internal/Solve.h"

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

//convenience typedefs
typedef Matrix<Eigen::RowVectorXd> RowVectorXd;
typedef Matrix<Eigen::VectorXd> VectorXd;
typedef Matrix<Eigen::MatrixXd> MatrixXd;
}

#endif /* MATRIX_H */
