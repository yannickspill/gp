#ifndef INTERNAL_LOG_DETERMINANT_H
#define INTERNAL_LOG_DETERMINANT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

// traits are general because a determinant will always be a double
template <class Derived> struct traits<LogDeterminant<Derived> > {
  typedef typename Derived::scalar_type scalar_type;
  typedef scalar_type result_type;
};

// determinant of a matrix, specialization for the LDLT case
template <class DerivedMat>
class LogDeterminant
    <Decomposition
     <DerivedMat, LDLTPolicy> > : public MatrixBase
                                  <LogDeterminant
                                   <Decomposition<DerivedMat, LDLTPolicy> > > {
 public:
  typedef typename traits<LogDeterminant>::scalar_type scalar_type;
  typedef typename traits<LogDeterminant>::result_type result_type;

 private:
  Decomposition<DerivedMat, LDLTPolicy> data_;

 public:
  // constructor
  LogDeterminant(const Decomposition<DerivedMat, LDLTPolicy>& data)
      : data_(data) {}

  // actual computation
  result_type get() const {
    return data_.get().vectorD().array().abs().log().sum();
  }

  unsigned get_version() const { return data_.get_version(); }
};
}
}
#endif /* INTERNAL_DETERMINANT_H */
