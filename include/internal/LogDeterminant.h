#ifndef LOG_DETERMINANT_H
#define LOG_DETERMINANT_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Derived> struct traits<LogDeterminant<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef double result_type;
};

//determinant of a matrix, specialization for the LDLT case
template <typename Derived>
class LogDeterminant
    <Decomposition
     <Derived, LDLTPolicy> > : public MatrixBase
                               <LogDeterminant
                                <Decomposition<Derived, LDLTPolicy> > > {
 public:
  typedef typename traits
      <LogDeterminant<Decomposition<Derived, LDLTPolicy> > >::scalar_type
          scalar_type;
  typedef typename traits
      <LogDeterminant<Decomposition<Derived, LDLTPolicy> > >::result_type
          result_type;

 private:
  Decomposition<Derived, LDLTPolicy> data_;

 public:
  // constructor
  LogDeterminant(const Decomposition<Derived, LDLTPolicy>& data) : data_(data) {}

  // actual computation
  result_type get() const {
    return data_.get().vectorD().array().abs().log().sum();
  }

  unsigned get_version() const {
    return data_.get_version();
  }
};
}
}
#endif /* DETERMINANT_H */
