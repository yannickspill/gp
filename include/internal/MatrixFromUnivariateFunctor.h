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
    RowsAtCompileTime = InMat::RowsAtCompileTime,
    ColsAtCompileTime = Functor::ColsAtCompileTime
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
};

// apply functor to every row of matrix, yiedling a column vector or possibly a
// matrix, depending on the output of the functor
template <class Functor, class InMat>
class MatrixFromUnivariateFunctor
    : public MatrixBase<MatrixFromUnivariateFunctor<Functor, InMat> > {

 public:
  typedef typename traits<MatrixFromUnivariateFunctor>::scalar_type scalar_type;
  typedef typename traits<MatrixFromUnivariateFunctor>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixFromUnivariateFunctor>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixFromUnivariateFunctor>::ColsAtCompileTime,
  };

  static_assert(Functor::RowsAtCompileTime == 1,
                "Functor should return a row vector!");
  static_assert(Functor::nargs == 1, "Expecting univariate functor!");

 private:
  Functor func_;
  InMat mat_;

  typedef typename std::conditional<ColsAtCompileTime == 1, std::true_type,
                                    std::false_type>::type is_vector_t;

 public:
  explicit MatrixFromUnivariateFunctor(const Functor& func, const InMat& mat)
      : func_(func), mat_(mat) {
    std::cout << typeid(func_).name() << std::endl;
  }

  result_type get() const { return get(is_vector_t()); }

  unsigned get_version() const {
    return func_.get_version() + mat_.get_version();
  }

 private:
  // the univariate functor returns a scalar, construct a vector
  result_type get(std::true_type) const {
    auto eigenmat = mat_.get();
    result_type retval(eigenmat.rows());  // using vector constructor
    for (unsigned i = 0; i < eigenmat.rows(); ++i)
      retval(i) = func_(eigenmat.row(i));
    return retval;
  }

  // the univariate functor returns a row vector, construct a matrix
  result_type get(std::false_type) const {
    auto eigenmat = mat_.get();
    auto firstline = func_(eigenmat.row(0));
    result_type retval(eigenmat.rows(), firstline.cols());
    retval.row(0) = firstline;
    for (unsigned i = 1; i < eigenmat.rows(); ++i)
      retval.row(i) = func_(eigenmat.row(i));
    return retval;
  }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H */
