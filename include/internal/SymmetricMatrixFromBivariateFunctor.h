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
  // SelfAdjointView does not support a number of operators, e.g. scalar * mat
  // typedef Eigen::SelfAdjointView<matrix_type, Eigen::Upper> result_type;
  typedef matrix_type result_type;
};

// build matrix by applying bivariate functor to every combination of row pairs
// of the input matrix. Only works with functors that return a numeric (e.g.
// non-matrix) type.
template <class Functor, class InMat>
class SymmetricMatrixFromBivariateFunctor
    : public MatrixBase<SymmetricMatrixFromBivariateFunctor<Functor, InMat> > {

 public:
  typedef typename traits
      <SymmetricMatrixFromBivariateFunctor>::scalar_type scalar_type;
  typedef typename traits
      <SymmetricMatrixFromBivariateFunctor>::matrix_type matrix_type;
  typedef typename traits
      <SymmetricMatrixFromBivariateFunctor>::result_type result_type;
  enum {
    RowsAtCompileTime = traits
    <SymmetricMatrixFromBivariateFunctor>::RowsAtCompileTime,
    ColsAtCompileTime = traits
    <SymmetricMatrixFromBivariateFunctor>::ColsAtCompileTime,
  };
  static_assert(Functor::nargs == 2, "Expecting bivariate functor!");

 private:
  Functor func_;
  InMat mat_;
  struct Data {
    typename InMat::result_type mat_;
    result_type val_;
    Data(const Functor& func, const typename InMat::result_type mat)
        : mat_(mat), val_(fill_val(func)) {}

   private:
    result_type fill_val(const Functor& func) const {
      matrix_type retval(mat_.rows(), mat_.rows());
      for (unsigned i = 0; i < mat_.rows(); ++i)
        for (unsigned j = i; j < mat_.rows(); ++j) {
          retval(i, j) = scalar_type(func(mat_.row(i), mat_.row(j)));
          if (j > i) retval(j, i) = retval(i, j);
        }
      return retval;
    }
  };
  mutable std::shared_ptr<Data> data_;

 public:
  explicit SymmetricMatrixFromBivariateFunctor(const Functor& func,
                                               const InMat& mat)
      : func_(func), mat_(mat) {}

  result_type get() const {
    data_ = std::make_shared<Data>(func_, mat_.get());
    return data_->val_;
  }

  unsigned get_version() const {
    return func_.get_version() + mat_.get_version();
  }
};
}
}
#endif /* INTERNAL_SYMMETRIC_MATRIX_FROM_BIVARIATE_FUNCTOR_H */
