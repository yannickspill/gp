#ifndef INTERNAL_TRACE_H
#define INTERNAL_TRACE_H

#include "macros.h"
#include "internal/ScalarBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <typename Derived>
class Trace : public ScalarBase<Trace<Derived> > {
 public:
  // typedefs
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Derived::scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };

 private:
  Derived data_;

 public:
  // constructor
  Trace(const Derived& data) : data_(data) {}

  // actual computation
  result_type get() const { return data_.get().trace(); }

  unsigned get_version() const { return data_.get_version(); }
};
}
}
#endif /* INTERNAL_TRACE_H */
