#ifndef MATRIX_H
#define MATRIX_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Dense>

namespace GP {
namespace internal {

// specialize traits for Matrix
template <typename EigenType>
struct traits<Matrix<EigenType> > {
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
    result_type data_;

   public:
    //! Construct directly from underlying data type
    Matrix(const result_type& data) : data_(data) {}

    //! Construct from GP matrix expression, convert if needed
    template <class GPExpression>
    explicit Matrix(const MatrixBase<GPExpression>& expr)
        : data_(expr.eval()) {}

    //! Return bare Implemented type
    // Use with precaution as this loses track of any dependent Scalars.
    result_type eval() const { return data_; }
};

typedef Matrix<Eigen::VectorXd> GPVectorXd;
typedef Matrix<Eigen::MatrixXd> MatrixXd;

}
}
#endif /* MATRIX_H */
