#include "Scalar.h"
#include "Matrix.h"

#include "internal/Functor.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    //from scalar to scalar
    Scalar x(1.0);
    Scalar y(2.0);
    auto xy = x*y;
    auto f1 = internal::make_functor(xy, x);
    if (f1(1.0) != 2.0) return 1;
    if (f1(2.0) != 4.0) return 2;
    //from scalar to scalar using temp expression
    auto f2 = internal::make_functor(x*y, x, y);
    if (f2(1.0,5.0) != 5.0) return 3;
    if (f2(2,-5.0) != -10.0) return 4;
    //functor of vector to matrix product
    Eigen::MatrixXd mat(Eigen::MatrixXd::Random(3,4));
    MatrixXd gpmat(mat);
    Eigen::RowVectorXd lvec(Eigen::RowVectorXd::LinSpaced(3,0,1));
    RowVectorXd gplvec(lvec);
    Eigen::VectorXd rvec(Eigen::VectorXd::LinSpaced(4,0,1));
    VectorXd gprvec(rvec);
    auto f3 = internal::make_functor(gplvec*gpmat*gprvec, gplvec);
    if (f3(lvec) != lvec*mat*rvec) return 5;
    if (gplvec.get() != lvec) return 6;
    // change input, check if make_functor resets the vector to its original
    // value
    Eigen::RowVectorXd randvec(Eigen::RowVectorXd::Random(3));
    if (f3(randvec) != randvec*mat*rvec) return 7;
    if (gplvec.get() != lvec) return 8;
    // check type traits
    static_assert(std::is_same
                  <typename decltype(f1)::scalar_type, double>::value,
                  "f1 has wrong scalar type");
    static_assert(std::is_same<typename decltype(f1)::result_type,
                               double>::value,
                  "f1 has wrong result type");
    static_assert(std::is_same
                  <typename decltype(f2)::scalar_type, double>::value,
                  "f2 has wrong scalar type");
    static_assert(std::is_same<typename decltype(f2)::result_type,
                               double>::value,
                  "f2 has wrong result type");
    static_assert(std::is_same
                  <typename decltype(f3)::scalar_type, double>::value,
                  "f3 has wrong scalar type");
    static_assert(std::is_same<typename decltype(f3)::result_type,
                               decltype(randvec * mat * rvec)>::value,
                  "f3 has wrong result type");

    return 0;
}
