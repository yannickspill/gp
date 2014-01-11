#ifndef GPMATRIX_BASE_H
#define GPMATRIX_BASE_H

#include "macros.h"
#include "ForwardDeclarations.h"
#include "GPMatrixSum.h"

//! Base class for any matrix
template <class Derived>
class GPMatrixBase {

 public:
  // typedefs
  typedef typename traits<Derived>::result_type result_type;
  typedef typename traits<Derived>::scalar_type scalar_type;
  static const unsigned RowsAtCompileTime = result_type::RowsAtCompileTime;
  static const unsigned ColsAtCompileTime = result_type::ColsAtCompileTime;

 public:
  // allow casting to Derived
  // operator Derived&() { return static_cast<Derived&>(*this); }
  // operator const Derived&() { return static_cast<const Derived&>(*this); }

  template <class Rhs>
  const GPMatrixSum<Derived, Rhs> operator+=(const GPMatrixBase<Rhs>& rhs);
};

#endif /* GPMATRIX_BASE_H */
