#ifndef INTERNAL_TRANSPOSE_H
#define INTERNAL_TRANSPOSE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace GP {
namespace internal {

template <typename Derived>
class Transpose : public MatrixBase<Transpose<Derived> > {
 private:
  Derived data_;

 public:
  // typedefs
  typedef typename Derived::scalar_type scalar_type;
  typedef decltype(data_.get().transpose()) result_type;
  enum {
    RowsAtCompileTime = Derived::ColsAtCompileTime,
    ColsAtCompileTime = Derived::RowsAtCompileTime
  };
 public:
  // constructor
  Transpose(const Derived& data) : data_(data) {}

  // actual computation
  result_type get() const { return data_.get().transpose(); }

  unsigned get_version() const { return data_.get_version(); }
};
}
}
#endif /* INTERNAL_TRANSPOSE_H */
