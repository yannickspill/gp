#ifndef SOLVE_DECOMPOSED_MATRIX_H
#define SOLVE_DECOMPOSED_MATRIX_H

#include "macros.h"
#include "DoubleInputCachedVersionTracker.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <memory>

//! Using a matrix decomposition for A, solve for AX=B
//the point of using this class is to allow for caching of the result
template <class Decomposition, class BMATRIX>
class SolveDecomposedMatrix
    : public DoubleInputCachedVersionTracker<Decomposition, BMATRIX> {

    typedef DoubleInputCachedVersionTracker<Decomposition, BMATRIX> P;

   public:
    //this result_type only works for square matrix decompositions.
    typedef typename Eigen::MatrixXd result_type;

   private:
    struct Data {
        Decomposition decomp_;
        BMATRIX B_;
        mutable result_type AmB_;
        Data(Decomposition decomp, BMATRIX B) : decomp_(decomp), B_(B) {}
    };
    std::shared_ptr<Data> data_;
  
   public:
    //! constructor
    SolveDecomposedMatrix(Decomposition decomp, BMATRIX B)
        : P(decomp, B), data_(std::make_shared<Data>(decomp, B)) {}

    result_type get() const {
        if (P::cache_is_invalid()) {
            data_->AmB_ = data_->decomp_.solve(data_->B_.get());
            P::set_cache_is_valid();
        }
        return data_->AmB_;
    }
};



#endif /* SOLVE_DECOMPOSED_MATRIX_H */
