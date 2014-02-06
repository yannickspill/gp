#ifndef INTERNAL_SCALAR_H
#define INTERNAL_SCALAR_H

#include "internal/ScalarBase.h"

#include <memory>
#include <Eigen/Core>

namespace GP {
namespace internal {

// traits
template <> struct traits<Scalar> {
  typedef double scalar_type;
  typedef double result_type;  // passed as value in get()
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
};

// modifiable double
class Scalar : public ScalarBase<Scalar> {
 public:
  // typedefs
  typedef typename traits<Scalar>::scalar_type scalar_type;
  typedef typename traits<Scalar>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<Scalar>::RowsAtCompileTime,
    ColsAtCompileTime = traits<Scalar>::ColsAtCompileTime
  };

 private:
  struct Data {
    double value_;
    unsigned version_;
    Data(double v) : value_(v), version_(0) {}
  };
  std::shared_ptr<Data> data_;

 public:
  explicit Scalar(double value) : data_(std::make_shared<Data>(value)) {}

  //! Explicit cast from any GP scalar expression
  template <class GPExpression>
  explicit Scalar(const ScalarBase<GPExpression>& expr)
      : data_(std::make_shared<Data>(expr.asDerived().get())) {}

  //pass as value because it is a double
  result_type get() const { return data_->value_; }

  unsigned get_version() const { return data_->version_; }

  void set(double value) {
    data_->value_ = value;
    (data_->version_)++;
  }
  template <class XprType, int BlockRows = Eigen::Dynamic,
            int BlockCols = Eigen::Dynamic>
  void set(const Eigen::Block<XprType, BlockRows, BlockCols>& b) {
      CHECK(b.rows() == 1 && b.cols() == 1,
              "input to Scalar should be castable to scalar_type");
      set(b(0,0));
  }

  bool operator==(const Scalar& other) const { return data_ == other.data_; }
  bool operator!=(const Scalar& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_SCALAR_H */
