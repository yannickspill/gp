#include "Scalar.h"
#include "Matrix.h"
#include "covariance_functions.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    //function returns double
    Scalar x(1.0);
    Scalar y(2.0);
    auto f1 = internal::make_functor(x*y, x, y);
    Eigen::VectorXd Vec(Eigen::VectorXd::LinSpaced(5,0,1));
    VectorXd inVec(Vec);
    auto m1 = MatrixXd::SymmetricApply(f1, inVec);
    Eigen::MatrixXd expected(5,5);
    for (unsigned i=0; i<5; i++)
        for (unsigned j=i; j<5; j++)
            expected(i,j) = Vec(i)*Vec(j);
    if (Eigen::MatrixXd(m1.get()) !=
            Eigen::MatrixXd(expected.selfadjointView<Eigen::Upper>()))
      return 1;
    //function returns 1x1 matrix
    Eigen::RowVectorXd left(Eigen::RowVectorXd::Random(5));
    Eigen::RowVectorXd right(Eigen::RowVectorXd::Random(5));
    RowVectorXd vleft(left), vright(right);
    auto f2 = internal::make_functor(vleft*vright.transpose(), vleft, vright);
    Eigen::MatrixXd Mat(Eigen::MatrixXd::Random(3,5));
    MatrixXd inMat(Mat);
    auto m2 = MatrixXd::SymmetricApply(f2, inMat);
    Eigen::MatrixXd expected2(3,3);
    for (unsigned i=0; i<3; i++)
        for (unsigned j=i; j<3; j++)
            expected2(i,j) = Mat.row(i)*Mat.row(j).transpose();
    if ((Eigen::MatrixXd(m2.get())
         - Eigen::MatrixXd(expected2.selfadjointView<Eigen::Upper>())).norm()
        > 1e-5)
      return 2;
    //product with a scalar
    Scalar lambda(1.0);
    //auto f3 = GP::covariance::squared_exponential(lambda);
    Scalar ax(1.0),ay(1.0);
    //auto f3 = GP::internal::make_functor(expression, ax, ay);
    auto f3 = GP::internal::Functor<Scalar,Scalar>(ax,ax);
    std::cout << f3(0.) << std::endl;
    GP::VectorXd inVec2(Eigen::VectorXd::LinSpaced(11,0,1));
    auto m3 = MatrixXd::Apply(f3,inVec2);
    auto val = (m3*3).get();
    std::cout << val << std::endl;
    /*
    auto m3 = MatrixXd::SymmetricApply(f3,inVec2);
    std::cout << "begin test" << std::endl;
        auto val = (lambda*m3).get();
    std::cout << "val" << std::endl;
    std::cout << val << std::endl;
    */
    //if ((1.*m3).get() != m3.get()) return 3;
    return 0;
}
