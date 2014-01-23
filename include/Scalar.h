#ifndef SCALAR_H
#define SCALAR_H

// scalar ops
#include "internal/Scalar.h"
#include "internal/ScalarScalarSum.h"
#include "internal/ScalarScalarDifference.h"
#include "internal/ScalarScalarProduct.h"
#include "internal/ScalarScalarQuotient.h"
#include "internal/ScalarOpposite.h"

// scalar builtin ops
#include "internal/ScalarBuiltinSum.h"
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
