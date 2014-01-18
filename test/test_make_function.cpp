#include "internal/Scalar.h"
#include "internal/ScalarScalarProduct.h"
#include "internal/MatrixScalarProduct.h"
#include "internal/MatrixMatrixProduct.h"
#include "internal/Matrix.h"

#include "internal/make_function.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP::internal;

int main(int, char*[]){
    Scalar x(1.0);
    Scalar y(2.0);
    auto xy = x*y;
    auto f1 = make_function(xy, x);
    if (f1(1.0) != 2.0) return 1;
    if (f1(2.0) != 4.0) return 2;
    auto f2 = make_function(x*y, x, y);
    if (f2(1.0,5.0) != 5.0) return 3;
    if (f2(2,-5.0) != -10.0) return 4;
    Eigen::MatrixXd mat(Eigen::MatrixXd::Random(3,4));
    MatrixXd gpmat(mat);
    Eigen::RowVectorXd lvec(Eigen::RowVectorXd::LinSpaced(3,0,1));
    RowVectorXd gplvec(lvec);
    Eigen::VectorXd rvec(Eigen::VectorXd::LinSpaced(4,0,1));
    VectorXd gprvec(rvec);
    auto f3 = make_function(gplvec*gpmat*gprvec, gplvec);
    if (f3(lvec) != lvec*mat*rvec) return 5;

    return 0;
}