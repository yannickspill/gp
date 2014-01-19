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
class LogDeterminant : public MatrixBase<LogDeterminant<Derived> > {
 public:
  typedef typename traits<LogDeterminant>::scalar_type scalar_type;
  typedef typename traits <LogDeterminant>::result_type result_type;

 private:
  Derived data_;

 public:
  // constructor
  LogDeterminant(const Derived& data) : data_(data) {}

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
