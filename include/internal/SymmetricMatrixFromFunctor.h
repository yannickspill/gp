#ifndef INTERNAL_SYMMETRIC_MATRIX_FROM_FUNCTOR_H
#define INTERNAL_SYMMETRIC_MATRIX_FROM_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class InMat, class FExpr, class... FArgs>
struct traits<SymmetricMatrixFromFunctor<InMat, FExpr, FArgs...> > {
  typedef typename Functor<InMat, FExpr, FArgs...>::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat::result_type::RowsAtCompileTime,
    ColsAtCompileTime = RowsAtCompileTime
  };
  typedef Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> matrix_type;
  typedef Eigen::SelfAdjointView<matrix_type, Eigen::Upper> result_type;
};

template <class InMat, class FExpr, class... FArgs>
class SymmetricMatrixFromFunctor : public MatrixBase
                          <SymmetricMatrixFromFunctor<InMat, FExpr, FArgs...> > {

 public:
  typedef typename traits<SymmetricMatrixFromFunctor>::scalar_type scalar_type;
  typedef typename traits<SymmetricMatrixFromFunctor>::matrix_type matrix_type;
  typedef typename traits<SymmetricMatrixFromFunctor>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<SymmetricMatrixFromFunctor>::RowsAtCompileTime,
      ColsAtCompileTime=traits<SymmetricMatrixFromFunctor>::ColsAtCompileTime,
  };

 private:
  Functor<FExpr, FArgs...> func_;
  InMat mat_;
  mutable std::shared_ptr<matrix_type> ret_;

 public:
  explicit SymmetricMatrixFromFunctor(const Functor<FExpr, FArgs...>& func,
                              const InMat& mat)
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
#endif /* INTERNAL_SYMMETRIC_MATRIX_FROM_FUNCTOR_H */
