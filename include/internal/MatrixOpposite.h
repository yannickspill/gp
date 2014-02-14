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

// expression template for the opposite of a valrix
template <class MatrixExpression>
class MatrixOpposite : public MatrixBase
                             <MatrixOpposite<MatrixExpression> > {
 public:
  // typedefs
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef decltype(
      - std::declval<typename MatrixExpression::result_type>()) result_type;
  enum {
    RowsAtCompileTime = MatrixExpression::RowsAtCompileTime,
    ColsAtCompileTime = MatrixExpression::ColsAtCompileTime
  };

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
