#include "GPMatrix.h"
#include "GPMatrixSum.h"
#include "GPMatrixDifference.h"
#include "GPMatrixProduct.h"

#include <Eigen/Dense>
#include <type_traits>

int main(int, char * []) {
  const unsigned int szx=10,szy=3;
  Eigen::MatrixXd x(Eigen::MatrixXd::Constant(szx,szy,1));
  GPMatrixXd vx(x);
  GPMatrixXd vy(Eigen::MatrixXd::Constant(szx,szy,2));
  if (vx.eval() != x) return 1;
  vx+vy;
  const Eigen::MatrixXd tmp((vx+vy).eval());
  GPMatrixXd vsum_manual(tmp);
  GPMatrixXd vsum(vy+vx);
  if (vsum.eval() != Eigen::MatrixXd::Constant(szx,szy,3)) return 2;
  if ((vx+vy).eval() != Eigen::MatrixXd::Constant(szx,szy,3)) return 3;

  // traits classes
  static_assert(
      std::is_same<GPMatrixXd::result_type, Eigen::MatrixXd >::value,
      "wrong result_type");
  static_assert(std::is_same<GPMatrixXd::scalar_type, double>::value,
                "wrong scalar_type");
  static_assert(
      std::is_same<decltype(vsum.eval()), Eigen::MatrixXd >::value,
      "wrong eval() return type");
  GPMatrixSum<GPMatrixXd,GPMatrixXd> s(vx,vy); //type is defined
  if (s.eval() != vsum.eval()) return 4; // works as expected
  GPMatrixXd vxt(x.transpose());
  if ((vxt*vx).eval() != x.transpose()*x) return 5;
  if ((vx-vy).eval() != Eigen::MatrixXd::Constant(szx,szy,-1)) return 6;
  return 0;
}
