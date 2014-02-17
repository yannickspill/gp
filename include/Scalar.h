#ifndef SCALAR_H
#define SCALAR_H

#include "internal/GPBase.h"
#include "internal/ScalarBase.h"
#include "internal/Scalar.h"

#include "internal/UnaryExpr.h"
#include "internal/UnaryOperations.h"

#include "internal/BinaryExpr.h"
#include "internal/BinaryOperations.h"

#include "internal/ScalarFromMatrix.h"
#include "internal/ConstScalar.h"

namespace GP {
// promote Scalar to GP namespace
using internal::Scalar;
}

#endif /* SCALAR_H */
