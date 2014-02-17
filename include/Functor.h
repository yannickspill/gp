#ifndef FUNCTOR_H
#define FUNCTOR_H

#include "internal/Functor.h"
#include "internal/MatrixFromUnivariateFunctor.h"
#include "internal/MatrixFromBivariateFunctor.h"
#include "internal/SymmetricMatrixFromBivariateFunctor.h"

namespace GP {
// promote make_functor to GP namespace
using internal::make_functor;
}
#endif /* FUNCTOR_H */
