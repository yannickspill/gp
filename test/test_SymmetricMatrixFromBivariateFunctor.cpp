#include "Scalar.h"
#include "Matrix.h"
#include "covariance_functions.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char * []) {
  // function returns double
  Scalar x(1.0);
  Scalar y(2.0);
  auto f1 = internal::make_functor(x * y, x, y);
  Eigen::VectorXd Vec(Eigen::VectorXd::LinSpaced(5, 0, 1));
  VectorXd inVec(Vec);
  auto m1 = MatrixXd::SymmetricApply(f1, inVec);
  Eigen::MatrixXd expected(5, 5);
  for (unsigned i = 0; i < 5; i++)
    for (unsigned j = i; j < 5; j++) expected(i, j) = Vec(i) * Vec(j);
  if (Eigen::MatrixXd(m1.get())
      != Eigen::MatrixXd(expected.selfadjointView<Eigen::Upper>()))
    return 1;
  std::cout << Eigen::MatrixXd(m1.get()) << std::endl;
  std::cout << " --- " << std::endl;
  std::cout << Eigen::MatrixXd((x * m1).get()) << std::endl;
  std::cout << " --- " << std::endl;
  std::cout << x.get() * Eigen::MatrixXd(expected.selfadjointView<Eigen::Upper>()) << std::endl;
  std::cout << " --- " << std::endl;
  if (Eigen::MatrixXd((x * m1).get())
      != x.get() * Eigen::MatrixXd(expected.selfadjointView<Eigen::Upper>()))
    return 2;
  if (Eigen::MatrixXd((3 * m1).get())
      != 3 * Eigen::MatrixXd(expected.selfadjointView<Eigen::Upper>()))
    return 3;
  // function returns 1x1 matrix
  Eigen::RowVectorXd left(Eigen::RowVectorXd::Random(5));
  Eigen::RowVectorXd right(Eigen::RowVectorXd::Random(5));
  RowVectorXd vleft(left), vright(right);
  auto f2 = internal::make_functor(vleft * vright.transpose(), vleft, vright);
  Eigen::MatrixXd Mat(Eigen::MatrixXd::Random(3, 5));
  MatrixXd inMat(Mat);
  auto m2 = MatrixXd::SymmetricApply(f2, inMat);
  Eigen::MatrixXd expected2(3, 3);
  for (unsigned i = 0; i < 3; i++)
    for (unsigned j = i; j < 3; j++)
      expected2(i, j) = Mat.row(i) * Mat.row(j).transpose();
  if ((Eigen::MatrixXd(m2.get())
       - Eigen::MatrixXd(expected2.selfadjointView<Eigen::Upper>())).norm()
      > 1e-5)
    return 4;
  if ((Eigen::MatrixXd((x * m2).get())
       - x.get() * Eigen::MatrixXd(expected2.selfadjointView<Eigen::Upper>()))
          .norm() > 1e-5)
    return 5;
  if ((Eigen::MatrixXd((3 * m2).get())
       - 3 * Eigen::MatrixXd(expected2.selfadjointView<Eigen::Upper>())).norm()
      > 1e-5)
    return 6;
  //more ops
  if ((Eigen::MatrixXd((inVec.transpose() * m2 * inVec).get())
       - Vec.transpose()
         * Eigen::MatrixXd(expected2.selfadjointView<Eigen::Upper>())
         * Vec).norm() > 1e-5)
    return 7;
  return 0;
}
