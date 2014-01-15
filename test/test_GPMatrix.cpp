#include "internal/Matrix.h"
#include "internal/MatrixSum.h"
#include "internal/MatrixDifference.h"
#include "internal/MatrixMatrixProduct.h"

#include <Eigen/Dense>
#include <type_traits>

using namespace GP::internal;

int main(int, char * []) {
  const unsigned int szx=10,szy=3;
  Eigen::MatrixXd x(Eigen::MatrixXd::Constant(szx,szy,1));
  MatrixXd vx(x);
  MatrixXd vy(Eigen::MatrixXd::Constant(szx,szy,2));
  if (vx.eval() != x) return 1;
  vx+vy;
  const Eigen::MatrixXd tmp((vx+vy).eval());
  MatrixXd vsum_manual(tmp);
  MatrixXd vsum(vy+vx);
  if (vsum.eval() != Eigen::MatrixXd::Constant(szx,szy,3)) return 2;
  if ((vx+vy).eval() != Eigen::MatrixXd::Constant(szx,szy,3)) return 3;

  // traits classes
  static_assert(
      std::is_same<MatrixXd::result_type, Eigen::MatrixXd >::value,
      "wrong result_type");
  static_assert(std::is_same<MatrixXd::scalar_type, double>::value,
                "wrong scalar_type");
  static_assert(
      std::is_same<decltype(vsum.eval()), Eigen::MatrixXd >::value,
      "wrong eval() return type");
  GP::internal::MatrixSum<MatrixXd,MatrixXd> s(vx,vy); //type is defined
  if (s.eval() != vsum.eval()) return 4; // works as expected
  MatrixXd vxt(x.transpose());
  if ((vxt*vx).eval() != x.transpose()*x) return 5;
  if ((vx-vy).eval() != Eigen::MatrixXd::Constant(szx,szy,-1)) return 6;
  return 0;
}
