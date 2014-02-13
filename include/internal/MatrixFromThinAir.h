#ifndef INTERNAL_MATRIX_FROM_THIN_AIR_H
#define INTERNAL_MATRIX_FROM_THIN_AIR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template<>
struct traits<MatrixFromThinAir> {
  typedef double scalar_type;
  typedef Eigen::MatrixXd result_type;
  enum {
    RowsAtCompileTime = 5,
    ColsAtCompileTime = 3
  };
};

// apply functor to every row of matrix, yiedling a column vector or possibly a
// matrix, depending on the output of the functor
class MatrixFromThinAir
    : public MatrixBase<MatrixFromThinAir> {

 public:
  typedef typename traits<MatrixFromThinAir>::scalar_type scalar_type;
  typedef typename traits<MatrixFromThinAir>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixFromThinAir>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixFromThinAir>::ColsAtCompileTime,
  };

 private:
  struct Data {
    result_type val_;
    Data(const result_type& val) : val_(val) {
      std::cout << "MFTA Data constructor, at " << this << std::endl;
      std::cout << "   MFTA val at " << &val_ << std::endl;
    }
  };
  mutable std::shared_ptr<Data> data_;

 public:
  explicit MatrixFromThinAir() : data_(nullptr) {
    std::cout << "MFTA created at " << this << std::endl;
  }

  MatrixFromThinAir(const MatrixFromThinAir& other)
      : data_(other.data_) {
    std::cout << "MFTA copied from " << &other << " to " << this << std::endl;
    std::cout << "   MFTA data at " << data_ << std::endl;
  }

  ~MatrixFromThinAir() {
    std::cout << "MFTA destructor " << this << std::endl;
  }

  result_type get() const {
      result_type retval(Eigen::MatrixXd::Random(5,3));
      data_ = std::make_shared<Data>(retval);
      return data_->val_;
  }

  unsigned get_version() const { return 0; }

};
}
}
#endif /* INTERNAL_MATRIX_FROM_THIN_AIR_H */
