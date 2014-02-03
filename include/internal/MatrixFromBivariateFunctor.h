#ifndef INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H
#define INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class Functor, class InMat1, class InMat2>
struct traits<MatrixFromBivariateFunctor<Functor, InMat1, InMat2> > {
  typedef typename Functor::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat1::result_type::RowsAtCompileTime,
    ColsAtCompileTime = InMat2::result_type::RowsAtCompileTime,
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
};

//apply functor to every row of both matrices
template <class Functor, class InMat1, class InMat2>
class MatrixFromBivariateFunctor
    : public MatrixBase<MatrixFromBivariateFunctor<Functor, InMat1, InMat2> > {

 public:
  typedef typename traits<MatrixFromBivariateFunctor>::scalar_type scalar_type;
  typedef typename traits<MatrixFromBivariateFunctor>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<MatrixFromBivariateFunctor>::RowsAtCompileTime,
      ColsAtCompileTime=traits<MatrixFromBivariateFunctor>::ColsAtCompileTime,
  };

 private:
  Functor func_;
  InMat1 mat1_;
  InMat2 mat2_;

 public:
  explicit MatrixFromBivariateFunctor(const Functor& func,
          const InMat1& mat1, const InMat2& mat2)
      : func_(func), mat1_(mat1), mat2_(mat2) {}

  result_type get() const {
    auto mat1 = mat1_.get();
    auto mat2 = mat2_.get();
    result_type retval(mat1.rows(), mat2.rows());
    for (unsigned i = 0; i < mat1.rows(); ++i)
        for (unsigned j = 0; j < mat2.rows(); ++j)
            retval(i,j) = func_(mat1.row(i), mat2.row(j));
    return retval;
  }

  unsigned get_version() const {
      return func_.get_version() + mat1_.get_version() + mat2_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H */
