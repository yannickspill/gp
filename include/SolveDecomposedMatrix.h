#ifndef SOLVE_DECOMPOSED_MATRIX_H
#define SOLVE_DECOMPOSED_MATRIX_H

#include "macros.h"
#include "DoubleInputCachedVersionTracker.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>

//! Using a matrix decomposition for A, solve for AX=B
//the point of using this class is to allow for caching of the result
template <class Decomposition, class BMATRIX>
class SolveDecomposedMatrix
    : public DoubleInputCachedVersionTracker<Decomposition, BMATRIX> {

    typedef DoubleInputCachedVersionTracker<Decomposition, BMATRIX> P;

   private:
    Decomposition decomp_;
    BMATRIX B_;

   public:
    //this result_type only works for square matrix decompositions.
    typedef typename Eigen::MatrixXd result_type;

   private:
    mutable result_type AmB_;
  
   public:
    //! constructor
    SolveDecomposedMatrix(Decomposition decomp, BMATRIX B)
        : P(decomp,B),
          decomp_(decomp),
          B_(B) {}

    result_type get() const {
        if (P::cache_is_invalid()) {
            AmB_ = decomp_.solve(B_.get());
            P::set_cache_is_valid();
        }
        return AmB_;
    }
};



#endif /* SOLVE_DECOMPOSED_MATRIX_H */
