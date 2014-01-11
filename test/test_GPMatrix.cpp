#include "GPMatrix.h"
#include "GPMatrixSum.h"

#include <Eigen/Dense>
#include <type_traits>

int main(int, char * []) {
  // traits classes
  static_assert(std::is_same<GPMatrixXd::result_type, Eigen::MatrixXd>::value,
                "wrong result_type");
  static_assert(std::is_same<GPMatrixXd::scalar_type, double>::value,
                "wrong scalar_type");
  // implementation
  Eigen::MatrixXd X(Eigen::MatrixXd::Random(10, 3));
  GPMatrixXd gpX(X);  // matrix creation
  if (gpX.eigen() != X) return 1;
  Eigen::MatrixXd Y(Eigen::MatrixXd::Random(10, 3));
  GPMatrixXd gpSum(X + Y);
  static_assert(std::is_same<decltype(gpSum.eigen()), Eigen::MatrixXd>::value,
                "wrong eigen() return type");
  if (gpSum.eigen() != X + Y) return 2;  // make sure nothing changes
  GPMatrixXd gpY(Y);
  gpY;
  //GPMatrixSum<GPMatrixXd,GPMatrixXd> s(gpX,gpY); //type is defined
  // if (s.eigen() != X+Y) return 3; // works as expected
  // gpX + gpY; //operator+
  // if ((gpX+gpY).eigen() != X+Y) return 4; // + is properly implemented
  // gpSum = gpX + gpY; //type conversion
  return 0;
}
