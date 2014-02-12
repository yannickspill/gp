#ifndef INTERNAL_MATRIX_BUILTIN_PRODUCT_H
#define INTERNAL_MATRIX_BUILTIN_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinProduct
template <class MatrixExpression>
struct traits<MatrixBuiltinProduct<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef decltype(2. * std::declval
                   <typename MatrixExpression::result_type>()) result_type;
  enum {
    RowsAtCompileTime = MatrixExpression::RowsAtCompileTime,
    ColsAtCompileTime = MatrixExpression::ColsAtCompileTime
  };
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinProduct : public MatrixBase
                             <MatrixBuiltinProduct<MatrixExpression> > {
 public:
  // typedefs
  typedef typename traits<MatrixBuiltinProduct>::scalar_type scalar_type;
  typedef typename traits<MatrixBuiltinProduct>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixBuiltinProduct>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixBuiltinProduct>::ColsAtCompileTime
  };

 private:
  double lhs_;
  MatrixExpression rhs_;

 public:
  // constructor
  MatrixBuiltinProduct(double lhs, const MatrixExpression& rhs)
      : lhs_(lhs), rhs_(rhs) {
    std::cout << "MBP created at " << this << std::endl;
    std::cout << "   MBP double at " << &lhs_ << std::endl;
    std::cout << "   MBP matrix at " << &rhs_ << std::endl;
      }

        MatrixBuiltinProduct(const MatrixBuiltinProduct& other)
          : lhs_(other.lhs_), rhs_(other.rhs_) {
        std::cout << "MBP copied from " << &other << " to " << this
                  << std::endl;
    std::cout << "   MBP double at " << &lhs_ << std::endl;
    std::cout << "   MBP matrix at " << &rhs_ << std::endl;
      }

      ~MatrixBuiltinProduct() {
        std::cout << "MBP destructor " << this << std::endl;
      }

  // actual computation
  result_type get() const { return lhs_*rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_PRODUCT_H */
