#ifndef SCALAR_H
#define SCALAR_H

#include "UnaryOp.h"
#include "UnaryOperations.h"

#include "BinaryOp.h"
#include "BinaryOperations.h"

#include "ScalarFromMatrix.h"

// scalar ops
#include "internal/Scalar.h"
#include "internal/ConstScalar.h"
#include "internal/ScalarScalarDifference.h"
#include "internal/ScalarScalarProduct.h"
#include "internal/ScalarScalarQuotient.h"

// scalar builtin ops
#include "internal/ScalarBuiltinDifference.h"
#include "internal/BuiltinScalarDifference.h"
#include "internal/ScalarBuiltinProduct.h"
#include "internal/BuiltinScalarQuotient.h"
#include "internal/ScalarBuiltinQuotient.h"

namespace GP {
// promote Scalar to GP namespace
using internal::Scalar;
}

#endif /* SCALAR_H */
