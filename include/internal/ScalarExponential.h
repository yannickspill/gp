#ifndef INTERNAL_SCALAR_EXPONENTIAL_H
#define INTERNAL_SCALAR_EXPONENTIAL_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <math.h>

namespace GP {
namespace internal {

// expression template for the exponential of a scalar
template <class ScalarExpression>
class ScalarExponential : public ScalarBase
                             <ScalarExponential<ScalarExpression> > {
 public:
  typedef typename ScalarExpression::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 private:
  ScalarExpression val_;

 public:
  // constructor
  ScalarExponential(const ScalarExpression& val) : val_(val) {}

  // actual computation
  result_type get() const { return std::exp(val_.get()); }

  unsigned get_version() const { return val_.get_version(); }
};
}
}
#endif /* INTERNAL_SCALAR_EXPONENTIAL_H */
