#ifndef INTERNAL_MATRIX_H
#define INTERNAL_MATRIX_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>

namespace GP {
namespace internal {

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <typename EigenType>
class Matrix : public MatrixBase<Matrix<EigenType> > {

 public:
  // typedefs
  typedef typename EigenType::Scalar scalar_type;
  typedef EigenType result_type;
  enum {
    RowsAtCompileTime = EigenType::RowsAtCompileTime,
    ColsAtCompileTime = EigenType::ColsAtCompileTime
  };

 private:
  struct Data {
    result_type value_;
    unsigned version_;
    Data(const result_type& v) : value_(v), version_(0) {}
  };
  std::shared_ptr<Data> data_;

 public:
  //! Construct directly from underlying data type
  explicit Matrix(const result_type& data)
      : data_(std::make_shared<Data>(data)) {}

  //! Explicit cast from any GP matrix expression
  template <class GPExpression>
  explicit Matrix(const MatrixBase<GPExpression>& expr)
      : data_(std::make_shared<Data>(expr.asDerived().get())) {}

  //! Return bare Implemented type
  // Use with precaution as this loses track of any dependent Scalars.
  const result_type& get() const { return data_->value_; }

  unsigned get_version() const { return data_->version_; }

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
