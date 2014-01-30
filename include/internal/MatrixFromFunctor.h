#ifndef INTERNAL_MATRIX_FROM_FUNCTOR_H
#define INTERNAL_MATRIX_FROM_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class InMat, class FExpr, class... FArgs>
struct traits<MatrixFromFunctor<InMat, FExpr, FArgs...> > {
  typedef typename Functor<InMat, FExpr, FArgs...>::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat::result_type::RowsAtCompileTime,
    ColsAtCompileTime = Functor
    <InMat, FExpr, FArgs...>::result_type::ColsAtCompileTime
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
};

template <class InMat, class FExpr, class... FArgs>
class MatrixFromFunctor : public MatrixBase
                          <MatrixFromFunctor<InMat, FExpr, FArgs...> > {

 public:
  typedef typename traits<MatrixFromFunctor>::scalar_type scalar_type;
  typedef typename traits<MatrixFromFunctor>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<MatrixFromFunctor>::RowsAtCompileTime,
      ColsAtCompileTime=traits<MatrixFromFunctor>::ColsAtCompileTime,
  };

  static_assert(Functor<FExpr, FArgs...>::result_type::RowsAtCompileTime == 1,
                "Functor should return a row vector!");

 private:
  Functor<FExpr, FArgs...> func_;
  InMat mat_;

 public:
  explicit MatrixFromFunctor(const Functor<FExpr, FArgs...>& func,
                              const InMat& mat)
      : func_(func), mat_(mat) {}

  result_type get() const {
    auto eigenmat = mat_.get();
    auto firstline = func_(eigenmat.row(0));
    result_type retval(eigenmat.rows(), firstline.cols());
    retval.row(0) = firstline;
    for (unsigned i = 1; i < eigenmat.rows(); ++i) {
      retval.row(i) = func_(eigenmat.row(i));
    }
    return retval;
  }

  unsigned get_version() const {
      return func_.get_version() + mat_.get_version();
  }
};


}
}
#endif /* INTERNAL_MATRIX_FROM_FUNCTOR_H */
