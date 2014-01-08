#ifndef SOLVE_DECOMPOSED_MATRIX_H
#define SOLVE_DECOMPOSED_MATRIX_H

#include "macros.h"
#include "DoubleInputCachedVersionTracker.h"

#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <memory>

//! Using a matrix decomposition for A, solve for AX=B
//the point of using this class is to allow for caching of the result
template <class DecompositionType, class BMatrixType>
class SolveDecomposedMatrix
    : public DoubleInputCachedVersionTracker<DecompositionType, BMatrixType> {

    typedef DoubleInputCachedVersionTracker<DecompositionType, BMatrixType> P;

  public:
    //this result_type only works for square matrix decompositions.
    typedef typename Eigen::MatrixXd result_type;
    static const unsigned RowsAtCompileTime = result_type::RowsAtCompileTime;
    static const unsigned ColsAtCompileTime = result_type::ColsAtCompileTime;

  private:
    struct Data {
        DecompositionType decomp_;
        BMatrixType B_;
        mutable result_type AmB_;
        Data(DecompositionType decomp, BMatrixType B) : decomp_(decomp), B_(B) {}
    };
    std::shared_ptr<Data> data_;

  public:
    //! constructor
    SolveDecomposedMatrix(DecompositionType decomp, BMatrixType B)
        : P(decomp, B), data_(std::make_shared<Data>(decomp, B)) {}

    result_type get() const {
        LOG("Solve: get()");
        if (P::cache_is_invalid()) {
            data_->AmB_ = data_->decomp_.solve(data_->B_.get());
            P::set_cache_is_valid();
            LOG(" reset cache");
        }
        LOG(std::endl);
        return data_->AmB_;
    }
};



#endif /* SOLVE_DECOMPOSED_MATRIX_H */
