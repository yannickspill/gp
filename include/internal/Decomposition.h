#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<Decomposition<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::LDLT
      <typename Eigen::MatrixBase<typename Derived::result_type>::PlainObject>
          result_type;
};

//cholesky Decomposition decomposition. Uses lower part by default, see Eigen doc.
template <typename Derived>
class Decomposition : public MatrixBase<Decomposition<Derived> > {
 public:
  typedef typename traits<Decomposition<Derived> >::scalar_type scalar_type;
  typedef typename traits<Decomposition<Derived> >::result_type result_type;

 private:
  Derived data_;

 public:
  // constructor
  Decomposition(const Derived& data) : data_(data) {}

  // actual computation
  result_type get() const { return data_.get().ldlt(); }

  unsigned get_version() const {
    return data_.get_version();
  }
};
}
}
#endif /* DECOMPOSITION_H */
