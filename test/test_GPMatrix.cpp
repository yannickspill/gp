#include "GPMatrix.h"
#include "GPMatrixSum.h"

#include <Eigen/Dense>
#include <type_traits>

int main(int, char * []) {
  const unsigned int szx=10,szy=3;
  Eigen::MatrixXd x(Eigen::MatrixXd::Constant(szx,szy,1));
  GPMatrixXd vx(x);
  GPMatrixXd vy(Eigen::MatrixXd::Constant(szx,szy,2));
  if (vx.eigen() != x) return 1;
  vx+vy;
  const Eigen::MatrixXd tmp((vx+vy).eigen());
  GPMatrixXd vsum_manual(tmp);
  GPMatrixXd vsum(vy+vx);
  if (vsum.eigen() != Eigen::MatrixXd::Constant(szx,szy,3)) return 2;
  if ((vx+vy).eigen() != Eigen::MatrixXd::Constant(szx,szy,3)) return 3;

  // traits classes
  static_assert(
      std::is_same<GPMatrixXd::result_type, Eigen::MatrixXd >::value,
      "wrong result_type");
  static_assert(std::is_same<GPMatrixXd::scalar_type, double>::value,
                "wrong scalar_type");
  static_assert(
      std::is_same<decltype(vsum.eigen()), Eigen::MatrixXd >::value,
      "wrong eigen() return type");
  GPMatrixSum<GPMatrixXd,GPMatrixXd> s(vx,vy); //type is defined
  if (s.eigen() != vsum.eigen()) return 4; // works as expected
  return 0;
}
