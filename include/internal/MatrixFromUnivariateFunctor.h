#ifndef INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H
#define INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class Functor, class InMat>
struct traits<MatrixFromUnivariateFunctor<Functor, InMat> > {
  typedef typename Functor::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat::result_type::RowsAtCompileTime,
    ColsAtCompileTime = Functor::result_type::ColsAtCompileTime
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
};

//apply functor to every row of matrix, yiedling a column vector or possibly a
//matrix, depending on the output of the functor
template <class Functor, class InMat>
class MatrixFromUnivariateFunctor : public MatrixBase
                          <MatrixFromUnivariateFunctor<Functor, InMat> > {

 public:
  typedef typename traits<MatrixFromUnivariateFunctor>::scalar_type scalar_type;
  typedef typename traits<MatrixFromUnivariateFunctor>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<MatrixFromUnivariateFunctor>::RowsAtCompileTime,
      ColsAtCompileTime=traits<MatrixFromUnivariateFunctor>::ColsAtCompileTime,
  };

  static_assert(Functor::result_type::RowsAtCompileTime == 1,
                "Functor should return a row vector!");

 private:
  Functor func_;
  InMat mat_;

 public:
  explicit MatrixFromUnivariateFunctor(const Functor& func, const InMat& mat)
      : func_(func), mat_(mat) {}

  result_type get() const {
    auto eigenmat = mat_.get();
    auto firstline = func_(eigenmat.row(0));
    result_type retval(eigenmat.rows(), firstline.cols());
    retval.row(0) = firstline;
    for (unsigned i = 1; i < eigenmat.rows(); ++i)
      retval.row(i) = func_(eigenmat.row(i));
    return retval;
  }

  unsigned get_version() const {
      return func_.get_version() + mat_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H */
