#ifndef INTERNAL_SCALAR_H
#define INTERNAL_SCALAR_H

#include "internal/ScalarBase.h"

#include <memory>
#include <Eigen/Core>

namespace GP {
namespace internal {

// modifiable double
class Scalar : public ScalarBase<Scalar> {
 public:
  // typedefs
  typedef double scalar_type;
  typedef double result_type;  // passed as value in get()
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };

 private:
  struct Data {
    double value_;
    unsigned version_;
    Data(double v) : value_(v), version_(0) {
      std::cout << "Scalar::Data created at " << this << std::endl;
    }
    ~Data() { std::cout << "Scalar::Data destroyed " << this << std::endl; }
  };
  std::shared_ptr<Data> data_;

 public:
  explicit Scalar(double value) : data_(std::make_shared<Data>(value)) {
    std::cout << "Scalar created at " << this << std::endl;
    std::cout << "   Scalar data is " << data_ << std::endl;
  }

  Scalar(const Scalar& other) : data_(other.data_) {
    std::cout << "Scalar copied from " << &other << " to " << this << std::endl;
    std::cout << "   Scalar data is " << data_ << std::endl;
  }

  ~Scalar() { std::cout << "Scalar destructor " << this << std::endl; }

  //! Explicit cast from any GP scalar expression
  template <class GPExpression>
  explicit Scalar(const ScalarBase<GPExpression>& expr)
      : data_(std::make_shared<Data>(expr.asDerived().get())) {}

  // pass as value because it is a double
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
    set(b(0, 0));
  }

  bool operator==(const Scalar& other) const { return data_ == other.data_; }
  bool operator!=(const Scalar& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_SCALAR_H */
