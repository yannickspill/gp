#ifndef GPMATRIX_H
#define GPMATRIX_H

#include "macros.h"
#include "GPMatrixBase.h"

#include <Eigen/Dense>

// specialize traits for GPMatrix
template <class EigenType>
struct traits<GPMatrix<EigenType> > {
  typedef EigenType result_type;
  typedef typename EigenType::Scalar scalar_type;
};

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <class EigenType>
class GPMatrix : public GPMatrixBase<GPMatrix<EigenType> > {

 private:
  EigenType data_;

 public:
  typedef typename GPMatrixBase<GPMatrix>::result_type result_type;

 public:
  //! Construct directly from Eigen expression
  GPMatrix(const EigenType& data) : data_(data) {}

  //! Construct from GP matrix expression, convert if needed
  template <class OtherDerived>
  explicit GPMatrix(const GPMatrixBase<OtherDerived>& expr)
      : data_(expr.eigen()) {}

  //! Return bare Eigen type
  // Use with precaution as this loses track of any dependent Scalars.
  result_type eigen() const { return data_; }
};

//! \addtogroup Common matrix typedefs @{
typedef GPMatrix<Eigen::MatrixXd> GPMatrixXd;
typedef GPMatrix<Eigen::VectorXd> GPVectorXd;
//! @}

#endif /* GPMATRIX_H */
