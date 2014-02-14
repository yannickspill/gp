#ifndef INTERNAL_SCALAR_OPPOSITE_H
#define INTERNAL_SCALAR_OPPOSITE_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>

namespace GP {
namespace internal {

// expression template for the opposite of a valrix
template <class ScalarExpression>
class ScalarOpposite : public ScalarBase
                             <ScalarOpposite<ScalarExpression> > {
 public:
  // typedefs
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
  ScalarOpposite(const ScalarExpression& val) : val_(val) {}

  // actual computation
  result_type get() const { return - val_.get(); }

  unsigned get_version() const { return val_.get_version(); }
};
}
}
#endif /* INTERNAL_SCALAR_OPPOSITE_H */
