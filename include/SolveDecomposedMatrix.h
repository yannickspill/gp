#ifndef SOLVE_DECOMPOSED_MATRIX_H
#define SOLVE_DECOMPOSED_MATRIX_H

#include "macros.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>

//! Using a matrix decomposition for A, solve for AX=B
//the point of using this class is to allow for caching of the result
template <class Decomposition, class BMATRIX>
class SolveDecomposedMatrix {

   private:
    Decomposition decomp_;
    BMATRIX B_;
    unsigned vd_, vb_, version_;
    mutable bool cache_invalid_;

   public:
    //this result_type only works for square matrix decompositions.
    typedef typename Eigen::MatrixXd result_type;

   private:
    mutable result_type AmB_;
  
   public:
    //! constructor
    SolveDecomposedMatrix(Decomposition decomp, BMATRIX B)
        : decomp_(decomp),
          B_(B),
          vd_(decomp_.update()),
          vb_(B_.update()),
          version_(0),
          cache_invalid_(true) {}

    unsigned update() {
        unsigned vd = decomp_.update();
        unsigned vb = B_.update();
        if (vd != vd_ || vb != vb_)
        {
            cache_invalid_=true; //allows lazy updating of decomposition
            version_++;
        }
        vd_ = vd;
        vb_ = vb;
        return version_;
    }

    result_type get() const {
        if (cache_invalid_) {
            AmB_ = decomp_.solve(B_.get());
            cache_invalid_=false;
        }
        return AmB_;
    }
};



#endif /* SOLVE_DECOMPOSED_MATRIX_H */
