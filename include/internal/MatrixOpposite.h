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
    enum {
    RowsAtCompileTime = MatrixExpression::RowsAtCompileTime,
    ColsAtCompileTime = MatrixExpression::ColsAtCompileTime
  };

};

// expression template for the opposite of a valrix
template <class MatrixExpression>
class MatrixOpposite : public MatrixBase
                             <MatrixOpposite<MatrixExpression> > {
 public:
  // typedefs
  typedef typename traits<MatrixOpposite>::scalar_type scalar_type;
  typedef typename traits<MatrixOpposite>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixOpposite>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixOpposite>::ColsAtCompileTime
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
