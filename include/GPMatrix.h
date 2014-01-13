#ifndef GPMATRIX_H
#define GPMATRIX_H

#include "macros.h"
#include "GPMatrixBase.h"

#include <Eigen/Dense>

// specialize traits for GPMatrix
template <typename EigenType>
struct traits<GPMatrix<EigenType> > {
    typedef typename EigenType::Scalar scalar_type;
    typedef EigenType result_type;
};

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <typename EigenType>
class GPMatrix : public GPMatrixBase<GPMatrix<EigenType> > {

   public:
    typedef typename traits<GPMatrix>::scalar_type scalar_type;
    typedef typename traits<GPMatrix>::result_type result_type;

   private:
    result_type data_;

   public:
    //! Construct directly from underlying data type
    GPMatrix(const result_type& data) : data_(data) {}

    //! Construct from GP matrix expression, convert if needed
    template <class GPExpression>
    explicit GPMatrix(const GPMatrixBase<GPExpression>& expr)
        : data_(expr.eigen()) {}

    //! Return bare Implemented type
    // Use with precaution as this loses track of any dependent Scalars.
    result_type eigen() const { return data_; }
};

typedef GPMatrix<Eigen::VectorXd> GPVectorXd;
typedef GPMatrix<Eigen::MatrixXd> GPMatrixXd;

#endif /* GPMATRIX_H */
