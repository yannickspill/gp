#ifndef GPMATRIX_H
#define GPMATRIX_H

#include "macros.h"
#include "GPMatrixBase.h"

#include <vector>

// specialize traits for GPMatrix
template <typename ScalarType>
struct traits<GPMatrix<ScalarType> > {
    typedef ScalarType scalar_type;
    typedef std::vector<scalar_type> result_type;
};

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <typename ScalarType>
class GPMatrix : public GPMatrixBase<GPMatrix<ScalarType> > {

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
        : data_(static_cast<GPExpression>(expr).eigen()) {}

    //! Return bare Implemented type
    // Use with precaution as this loses track of any dependent Scalars.
    result_type eigen() const { return data_; }
};

typedef GPMatrix<double> GPVector;

#endif /* GPMATRIX_H */
