#ifndef INTERNAL_CONST_SCALAR_H
#define INTERNAL_CONST_SCALAR_H

#include "internal/ScalarBase.h"

#include <memory>
#include <Eigen/Core>

namespace GP {
namespace internal {

// non-modifiable Scalar, used as wrapper to pass doubles around using implicit
// conversion
class ConstScalar : public ScalarBase<ConstScalar> {
 public:
  // typedefs
  typedef double scalar_type;
  typedef double result_type;  // passed as value in get()
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };

 private:
  double val_;

 public:
  ConstScalar(double value) : val_(value) {}

  // pass as value because it is a double
  result_type get() const { return val_; }

  unsigned get_version() const { return 0; }

  bool operator==(const ConstScalar& other) const { return val_ == other.val_; }
  bool operator!=(const ConstScalar& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_CONST_SCALAR_H */
