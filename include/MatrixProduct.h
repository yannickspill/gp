#ifndef MATRIX_PRODUCT_H
#define MATRIX_PRODUCT_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>

//! Product of two Eigen objects (or Eigen and Scalar)
template <class LEFTTYPE, class RIGHTTYPE>
class MatrixProduct {
    LEFTTYPE l_;
    RIGHTTYPE r_;
    unsigned vleft_, vright_, version_;

   public:
    //! constructor
    MatrixProduct(LEFTTYPE l, RIGHTTYPE r)
        : l_(l),
          r_(r),
          vleft_(l_.update()),
          vright_(r_.update()),
          version_(0) {}

    typedef decltype(l_.get() * r_.get()) result_type;
    result_type get() const { return l_.get() * r_.get(); }

    void set_lhs(LEFTTYPE l) {
        l_ = l;
        vleft_ = l_.update();
        version_++;
    }
    void set_rhs(RIGHTTYPE r) {
        r_ = r;
        vright_ = r_.update();
        version_++;
    }

    unsigned update() {
        unsigned vleft = l_.update();
        unsigned vright = r_.update();
        if (vleft != vleft_ || vright != vright_) version_++;
        vleft_ = vleft;
        vright_ = vright;
        return version_;
    }
};

#endif /* MATRIX_PRODUCT_H */
