#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<Transpose<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::Transpose<typename Derived::result_type> result_type;
};

// specialize for matrix, which needs to be constd
template <class EigenType> struct traits<Transpose<Matrix<EigenType> > > {
  typedef typename Matrix<EigenType>::scalar_type scalar_type;
  typedef typename Eigen::Transpose<const EigenType> result_type;
};

template <typename Derived>
class Transpose : public MatrixBase<Transpose<Derived> > {
 public:
  typedef typename traits<Transpose<Derived> >::scalar_type scalar_type;
  typedef typename traits<Transpose<Derived> >::result_type result_type;

 private:
  const Derived& data_;

 public:
  // constructor
  Transpose(const Derived& data) : data_(data) {}

  // actual computation
  result_type get() const { return data_.get().transpose(); }

  unsigned get_version() const { return data_.get_version(); }
};
}
}
#endif /* TRANSPOSE_H */
