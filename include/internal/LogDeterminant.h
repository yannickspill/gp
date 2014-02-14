#ifndef INTERNAL_LOG_DETERMINANT_H
#define INTERNAL_LOG_DETERMINANT_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

// determinant of a matrix, specialization for the LDLT case
template <class DerivedMat>
class LogDeterminant
    <Decomposition
     <DerivedMat, LDLTPolicy> > : public ScalarBase
                                  <LogDeterminant
                                   <Decomposition<DerivedMat, LDLTPolicy> > > {
 public:
  // typedefs
  typedef typename DerivedMat::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
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

// determinant of a matrix, specialization for the cached LDLT case
template <class DerivedMat>
class LogDeterminant
    <Cache
     <Decomposition
      <DerivedMat,
       LDLTPolicy> > > : public ScalarBase
                         <LogDeterminant
                          <Cache<Decomposition<DerivedMat, LDLTPolicy> > > > {
 public:
  typedef typename DerivedMat::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };

 private:
  Cache<Decomposition<DerivedMat, LDLTPolicy> > data_;

 public:
  // constructor
  LogDeterminant(const Cache<Decomposition<DerivedMat, LDLTPolicy> >& data)
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
