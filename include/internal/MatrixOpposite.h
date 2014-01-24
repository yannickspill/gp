#ifndef INTERNAL_MATRIX_OPPOSITE_H
#define INTERNAL_MATRIX_OPPOSITE_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// specialize traits for MatrixOpposite
template <class MatrixExpression>
struct traits<MatrixOpposite<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef decltype(
      - std::declval<typename MatrixExpression::result_type>()) result_type;
};

// expression template for the opposite of a valrix
template <class MatrixExpression>
class MatrixOpposite : public MatrixBase
                             <MatrixOpposite<MatrixExpression> > {
 public:
  typedef typename traits
      <MatrixOpposite<MatrixExpression> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixOpposite<MatrixExpression> >::result_type result_type;

 private:
  MatrixExpression val_;

 public:
  // constructor
  MatrixOpposite(const MatrixExpression& val) : val_(val) {}

  // actual computation
  result_type get() const { return - val_.get(); }

  unsigned get_version() const { return val_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_OPPOSITE_H */
