#ifndef INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H
#define INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

// apply functor to every row of matrix, yiedling a column vector or possibly a
// matrix, depending on the output of the functor
template <class Functor, class InMat>
class MatrixFromUnivariateFunctor
    : public MatrixBase<MatrixFromUnivariateFunctor<Functor, InMat> > {

 public:
  typedef typename Functor::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = InMat::RowsAtCompileTime,
    ColsAtCompileTime = Functor::ColsAtCompileTime
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime> result_type;
  static_assert(Functor::RowsAtCompileTime == 1,
                "Functor should return a row vector!");
  static_assert(Functor::nargs == 1, "Expecting univariate functor!");

 private:
  typedef typename std::conditional<ColsAtCompileTime == 1, std::true_type,
                                    std::false_type>::type is_vector_t;

  Functor func_;
  InMat mat_;
  struct Data {
    typename InMat::result_type mat_;
    result_type val_;
    Data(const Functor& func, const typename InMat::result_type& mat)
        : mat_(mat), val_(fill_val(func, is_vector_t())) {
      std::cout << "MFUF Data constructor, at " << this << std::endl;
      std::cout << "   MFUF val at " << &val_ << std::endl;
    }

   private:
    // the univariate functor returns a scalar, construct a vector
    result_type fill_val(const Functor& func, std::true_type) const {
      result_type retval(mat_.rows());  // using vector constructor
      for (unsigned i = 0; i < mat_.rows(); ++i) retval(i) = func(mat_.row(i));
      return retval;
      ;
    }

    // the univariate functor returns a row vector, construct a matrix
    result_type fill_val(const Functor& func, std::false_type) const {
      auto firstline = func(mat_.row(0));
      result_type retval(mat_.rows(), firstline.cols());
      retval.row(0) = firstline;
      for (unsigned i = 1; i < mat_.rows(); ++i)
        retval.row(i) = func(mat_.row(i));
      return retval;
    }
  };
  mutable std::shared_ptr<Data> data_;

 public:
  explicit MatrixFromUnivariateFunctor(const Functor& func, const InMat& mat)
      : func_(func), mat_(mat), data_(nullptr) {
    std::cout << "MFUF created at " << this << std::endl;
    std::cout << "   MFUF Functor at " << &func_ << std::endl;
    std::cout << "   MFUF InMat at " << &mat_ << std::endl;
  }

  MatrixFromUnivariateFunctor(const MatrixFromUnivariateFunctor& other)
      : func_(other.func_), mat_(other.mat_), data_(other.data_) {
    std::cout << "MFUF copied from " << &other << " to " << this << std::endl;
    std::cout << "   MFUF Functor at " << &func_ << std::endl;
    std::cout << "   MFUF InMat at " << &mat_ << std::endl;
    std::cout << "   MFUF data at " << &data_ << std::endl;
  }

  ~MatrixFromUnivariateFunctor() {
    std::cout << "MFUF destructor " << this << std::endl;
  }

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
#endif /* INTERNAL_MATRIX_FROM_UNIVARIATE_FUNCTOR_H */
