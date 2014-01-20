#ifndef MATRIX_H
#define MATRIX_H

// matrix ops
#include "internal/Matrix.h"
#include "internal/MatrixSum.h"
#include "internal/MatrixDifference.h"
#include "internal/MatrixMatrixProduct.h"
#include "internal/Transpose.h"
#include "internal/LDLTPolicy.h"
#include "internal/Decomposition.h"
#include "internal/LogDeterminant.h"
#include "internal/Solve.h"

// scalar ops
#include "internal/Scalar.h"
#include "internal/ScalarScalarSum.h"
#include "internal/ScalarScalarDifference.h"
#include "internal/ScalarScalarProduct.h"
#include "internal/ScalarScalarQuotient.h"

// scalar builtin ops
#include "internal/ScalarBuiltinSum.h"
#include "internal/ScalarBuiltinDifference.h"
#include "internal/BuiltinScalarDifference.h"
#include "internal/ScalarBuiltinProduct.h"
#include "internal/BuiltinScalarQuotient.h"
#include "internal/ScalarBuiltinQuotient.h"

// matrix builtin ops
#include "internal/MatrixBuiltinProduct.h"
#include "internal/MatrixBuiltinQuotient.h"

// matrix scalar ops
#include "internal/MatrixScalarProduct.h"
#include "internal/MatrixScalarQuotient.h"

//misc
#include "make_function.h"


//external libs
#include <Eigen/Dense>

namespace GP {
// promote some internal things to GP namespace
using internal::Matrix;
using internal::Scalar;
using internal::Transpose;
using internal::Decomposition;
using internal::LDLTPolicy;
using internal::LogDeterminant;
using internal::Solve;

//convenience typedefs
typedef Matrix<Eigen::RowVectorXd> RowVectorXd;
typedef Matrix<Eigen::VectorXd> VectorXd;
typedef Matrix<Eigen::MatrixXd> MatrixXd;
}

#endif /* MATRIX_H */
