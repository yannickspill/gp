#ifndef INTERNAL_GP_BASE_H
#define INTERNAL_GP_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"
#include <Eigen/Core>

namespace GP {
namespace internal {

//! Base class for all GP objects
template <class Derived> class GPBase {

 protected:
  GPBase() {}  // only children can instantiate it

 public:
  // typedefs
  typedef typename traits<Derived>::scalar_type scalar_type;
  typedef typename traits<Derived>::result_type result_type;

 public:
  // allow explicit casting to Derived
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }

  //caching
  Cache<Derived> cache() const {
      return Cache<Derived>(asDerived());
  }

  //Eigen stuff
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// Scalar * Matrix
template <class Scal, class Mat>
const MatrixScalarProduct<Mat, Scal> operator*(const ScalarBase<Scal>& lhs,
                                               const MatrixBase<Mat>& rhs) {
  return MatrixScalarProduct<Mat, Scal>(rhs.asDerived(), lhs.asDerived());
}
// Matrix * Scalar
template <class Mat, class Scal>
const MatrixScalarProduct<Mat, Scal> operator*(const MatrixBase<Mat>& lhs,
                                               const ScalarBase<Scal>& rhs) {
  return MatrixScalarProduct<Mat, Scal>(lhs.asDerived(), rhs.asDerived());
}
// Matrix / Scalar
template <class Mat, class Scal>
const MatrixScalarQuotient<Mat, Scal> operator/(const MatrixBase<Mat>& lhs,
                                               const ScalarBase<Scal>& rhs) {
  return MatrixScalarQuotient<Mat, Scal>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* INTERNAL_GP_BASE_H */
