#ifndef INTERNAL_APPLY_MATRIX_H
#define INTERNAL_APPLY_MATRIX_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class FArgs, class Other> struct traits<ApplyMatrix<FArgs, Other> > {
  typedef typename std::function<FArgs>::result_type::Scalar scalar_type;
  typedef typename Eigen::Matrix<scalar_type,
                typename Other::RowsAtCompileTime,
                typename std::function<FArgs>::result_type::ColsAtCompileTime
                > result_type;
};

//! Applies a function to every row of a matrix.
// will not check whether func is compatible with the rows of the matrix
// will only compile if the function returns a row vector
template <class FArgs, class Other>
class ApplyMatrix : public MatrixBase<ApplyMatrix<EigenType> > {

 public:
  typedef typename traits<ApplyMatrix>::scalar_type scalar_type;
  typedef typename traits<ApplyMatrix>::result_type result_type;
  static_assert(std::function<FArgs>::result_type::RowsAtCompileTime == 1,
          "Function should return a row vector!");

 private:
  std::function<FArgs> func_;
  Other mat_;

 public:
  explicit ApplyMatrix(const std::function<FArgs>& func, const Other& mat)
      : func_(func), mat_(mat) {}

  result_type get() const {
      auto firstline = func_(mat_.row(0));
      result_type retval(mat_.rows(), firstline.cols());
      retval.row(0) = firstline;
      for (unsigned i=1; i < mat_.rows(); ++i){
        retval.row(i) = func_(mat_.row(i));
      }
      return retval;
  }


  unsigned get_version() const { return mat_.get_version() +

  void set(const result_type& data) {
    data_->value_ = data;
    (data_->version_)++;
  }

  bool operator==(const ApplyMatrix& other) const { return data_ == other.data_; }
  bool operator!=(const ApplyMatrix& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_APPLY_MATRIX_H */
