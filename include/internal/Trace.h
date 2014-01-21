#ifndef INTERNAL_TRACE_H
#define INTERNAL_TRACE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<Trace<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Derived::scalar_type result_type;
};

template <typename Derived>
class Trace : public MatrixBase<Trace<Derived> > {
 public:
  typedef typename traits<Trace<Derived> >::scalar_type scalar_type;
  typedef typename traits<Trace<Derived> >::result_type result_type;

 private:
  const Derived& data_;

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
