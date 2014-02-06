#ifndef MEAN_FUNCTIONS_H
#define MEAN_FUNCTIONS_H

#include "Scalar.h"
#include "Matrix.h"
#include "Functor.h"

#include <Eigen/Dense>

namespace GP{

/*
//! Polynomial function
//\f[ f(\mathbf{X}; \mathbf{\beta}, b) = b + \mathbf{\beta}^\top\mathbf{X} \f]
template<class InMat>
class PolynomialFunction {

  typedef Eigen::DiagonalMatrix
      <typename InMat::scalar_type, InMat::ColsAtCompileTime> input_base_type;

    Scalar b_;
    InMat beta_;
    Matrix<input_base_type> x_;
    auto f_;

    public:
        PolynomialFunction(Scalar b, const InMat& beta) : b_(b), beta_(beta) {
        mk_functor();
        }

        void mk_functor() {
            x_ = Matrix<input_base_type>(input_base_type(beta_.cols()));

            f_ = make_functor((-0.5 * diff.transpose() * M_ * diff).exp(), xp_,
                              xq_);
        }

        template<class Vec1, class Vec2>
        double value(Vec1&& v1, Vec2&& v2) const {
            return f_(std::forward<Vec1>(v1), std::forward<Vec2>(v2));
        }
};
*/

struct ZeroFunction {
  template <class Vec> double value(Vec&&) const { return 0.; }
};
}
#endif /* MEAN_FUNCTIONS_H */
