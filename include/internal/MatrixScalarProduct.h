#ifndef MATRIX_SCALAR_PRODUCT_H
#define MATRIX_SCALAR_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Mat, class Scal>
struct traits<MatrixScalarProduct<Mat, Scal> > {
  static_assert(std::is_same
                <typename Mat::scalar_type, typename Scal::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Mat::scalar_type scalar_type;
  typedef typename Eigen::CwiseUnaryOp
      <Eigen::internal::scalar_multiple_op<scalar_type>,
       const typename Mat::result_type> result_type;
  typedef branch_tag node_type;
};

template <typename Mat, typename Scal>
class MatrixScalarProduct : public MatrixBase<MatrixScalarProduct<Mat, Scal> > {
 private:
  Mat lhs_;
  Scal rhs_;

 public:
  typedef typename traits
      <MatrixScalarProduct<Mat, Scal> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixScalarProduct<Mat, Scal> >::result_type result_type;
  typedef typename traits<MatrixScalarProduct<Mat, Scal> >::node_type node_type;

 public:
  // constructor
  MatrixScalarProduct(const Mat& lhs, const Scal& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() * rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* MATRIX_SCALAR_PRODUCT_H */
