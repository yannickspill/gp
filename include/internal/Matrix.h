#ifndef INTERNAL_MATRIX_H
#define INTERNAL_MATRIX_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>

namespace GP {
namespace internal {

template <typename EigenType> struct traits<Matrix<EigenType> > {
  typedef typename EigenType::Scalar scalar_type;
  typedef EigenType result_type;
};

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <typename EigenType>
class Matrix : public MatrixBase<Matrix<EigenType> > {

 public:
  typedef typename traits<Matrix>::scalar_type scalar_type;
  typedef typename traits<Matrix>::result_type result_type;

 private:
  struct Data {
    result_type value_;
    unsigned version_;
    Data(const result_type& v) : value_(v), version_(0) {
          std::cout << "Construct Matrix::Data " << this << std::endl;
    }
  };
  std::shared_ptr<Data> data_;

 public:
  //! Construct directly from underlying data type
  explicit Matrix(const result_type& data)
      : data_(std::make_shared<Data>(data)) {
          std::cout << "Construct Matrix " << this << " with data_ " << data_ << std::endl;
      }

  //! Explicit cast from any GP matrix expression
  template <class GPExpression>
  explicit Matrix(const MatrixBase<GPExpression>& expr)
      : data_(std::make_shared<Data>(expr.asDerived().get())) { }

  //! Return bare Implemented type
  // Use with precaution as this loses track of any dependent Scalars.
  const result_type& get() const {
          std::cout << "get Matrix " << this<< " with data_ " << data_ << std::endl;
      return data_->value_; }

  unsigned get_version() const {
          std::cout << "get Matrix version " << this << " with data_ " << data_ << std::endl;
      return data_->version_; }

  void set(const result_type& data) {
    data_->value_ = data;
    (data_->version_)++;
  }

  bool operator==(const Matrix& other) const { return data_ == other.data_; }
  bool operator!=(const Matrix& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_MATRIX_H */
