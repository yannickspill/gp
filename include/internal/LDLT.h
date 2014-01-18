#ifndef LDLT_H
#define LDLT_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"
#include "internal/tags.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<LDLT<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef typename Eigen::LDLT
      <typename Eigen::MatrixBase<typename Derived::result_type>::PlainObject>
          result_type;
  typedef branch_tag node_type;
};

//cholesky LDLT decomposition. Uses lower part by default, see Eigen doc.
template <typename Derived>
class LDLT : public MatrixBase<LDLT<Derived> > {
 public:
  typedef typename traits<LDLT<Derived> >::scalar_type scalar_type;
  typedef typename traits<LDLT<Derived> >::result_type result_type;
  typedef typename traits<LDLT<Derived> >::node_type node_type;

 private:
  Derived data_;

 public:
  // constructor
  LDLT(const Derived& data) : data_(data) {}

  // actual computation
  result_type get() const { return data_.get().ldlt(); }

  unsigned get_version() const {
    return data_.get_version();
  }
};
}
}
#endif /* LDLT_H */
