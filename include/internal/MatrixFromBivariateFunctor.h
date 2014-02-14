#ifndef INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H
#define INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

// apply functor to every row of both matrices
template <class Functor, class InMat1, class InMat2>
class MatrixFromBivariateFunctor
    : public MatrixBase<MatrixFromBivariateFunctor<Functor, InMat1, InMat2> > {

 public:
  typedef typename Functor::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat1::RowsAtCompileTime,
    ColsAtCompileTime = InMat2::RowsAtCompileTime,
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
  static_assert(Functor::nargs == 2, "Expecting bivariate functor!");

 private:
  Functor func_;
  InMat1 mat1_;
  InMat2 mat2_;
  struct Data {
    typename InMat1::result_type mat1_;
    typename InMat2::result_type mat2_;
    result_type val_;
    Data(const Functor& func, const typename InMat1::result_type mat1,
         const typename InMat2::result_type mat2)
        : mat1_(mat1), mat2_(mat2), val_(fill_val(func)) {}

   private:
    result_type fill_val(const Functor& func) const {
      result_type retval(mat1_.rows(), mat2_.rows());
      for (unsigned i = 0; i < mat1_.rows(); ++i)
        for (unsigned j = 0; j < mat2_.rows(); ++j)
          retval(i, j) = func(mat1_.row(i), mat2_.row(j));
      return retval;
    }
  };
  mutable std::shared_ptr<Data> data_;

 public:
  explicit MatrixFromBivariateFunctor(const Functor& func, const InMat1& mat1,
                                      const InMat2& mat2)
      : func_(func), mat1_(mat1), mat2_(mat2) {}

  result_type get() const {
    data_ = std::make_shared<Data>(func_, mat1_.get(), mat2_.get());
    return data_->val_;
  }

  unsigned get_version() const {
    return func_.get_version() + mat1_.get_version() + mat2_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_BIVARIATE_FUNCTOR_H */
