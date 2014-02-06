#ifndef INTERNAL_SYMMETRIC_MATRIX_FROM_BIVARIATE_FUNCTOR_H
#define INTERNAL_SYMMETRIC_MATRIX_FROM_BIVARIATE_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class Functor, class InMat>
struct traits<SymmetricMatrixFromBivariateFunctor<Functor, InMat> > {
  typedef typename Functor::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat::RowsAtCompileTime,
    ColsAtCompileTime = InMat::RowsAtCompileTime
  };
  typedef Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> matrix_type;
  typedef Eigen::SelfAdjointView<matrix_type, Eigen::Upper> result_type;
};

//build matrix by applying bivariate functor to every combination of row pairs
//of the input matrix. Only works with functors that return a numeric (e.g.
//non-matrix) type.
template <class Functor, class InMat>
class SymmetricMatrixFromBivariateFunctor : public MatrixBase
                          <SymmetricMatrixFromBivariateFunctor<Functor, InMat> > {

 public:
  typedef typename traits<SymmetricMatrixFromBivariateFunctor>::scalar_type scalar_type;
  typedef typename traits<SymmetricMatrixFromBivariateFunctor>::matrix_type matrix_type;
  typedef typename traits<SymmetricMatrixFromBivariateFunctor>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<SymmetricMatrixFromBivariateFunctor>::RowsAtCompileTime,
      ColsAtCompileTime=traits<SymmetricMatrixFromBivariateFunctor>::ColsAtCompileTime,
  };

 private:
  Functor func_;
  InMat mat_;
  mutable std::shared_ptr<matrix_type> ret_;

 public:
  explicit SymmetricMatrixFromBivariateFunctor(const Functor& func, const InMat& mat)
      : func_(func), mat_(mat), ret_(nullptr) {}

  result_type get() const {
    auto eigenmat = mat_.get();
    ret_ = std::make_shared
        <matrix_type>(matrix_type(eigenmat.rows(), eigenmat.rows()));
    for (unsigned i = 0; i < eigenmat.rows(); ++i)
        for (unsigned j = i; j < eigenmat.rows(); ++j)
            ret_->operator()(i,j)
                = scalar_type(func_(eigenmat.row(i), eigenmat.row(j)));
    return result_type(*ret_);
  }

  unsigned get_version() const {
      return func_.get_version() + mat_.get_version();
  }
};


}
}
#endif /* INTERNAL_SYMMETRIC_MATRIX_FROM_BIVARIATE_FUNCTOR_H */
