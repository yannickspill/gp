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

// scalar builtin ops
#include "internal/ScalarBuiltinProduct.h"

namespace GP {
// promote Scalar to GP namespace
using internal::Scalar;
}

#endif /* SCALAR_H */
