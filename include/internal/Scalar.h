#ifndef INTERNAL_SCALAR_H
#define INTERNAL_SCALAR_H

#include "internal/ScalarBase.h"

#include <memory>

namespace GP {
namespace internal {

// traits
template <> struct traits<Scalar> {
  typedef double scalar_type;
  typedef double result_type;  // passed as value in get()
  typedef leaf_tag node_type;
};

// modifiable double
class Scalar : public ScalarBase<Scalar> {
 public:
  typedef typename traits<Scalar>::scalar_type scalar_type;
  typedef typename traits<Scalar>::result_type result_type;
  typedef typename traits<Scalar>::node_type node_type;

 private:
  struct Data {
    double value_;
    unsigned version_;
    Data(double v) : value_(v), version_(0) {}
  };
  std::shared_ptr<Data> data_;

 public:
  explicit Scalar(double value) : data_(std::make_shared<Data>(value)) {}

  result_type get() const { return data_->value_; }

  unsigned get_version() const { return data_->version_; }

  void set(double value) {
    data_->value_ = value;
    (data_->version_)++;
  }

  bool operator==(const Scalar& other) const { return data_ == other.data_; }
  bool operator!=(const Scalar& other) const { return !(*this == other); }
};
}
}
#endif /* INTERNAL_SCALAR_H */
