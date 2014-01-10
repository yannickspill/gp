#include "GPMatrix.h"

#include <Eigen/Dense>

int main(int, char * []) {
  Eigen::MatrixXd X(Eigen::MatrixXd::Random(10, 3));
  GPMatrixXd gpX(X); // matrix creation
  if (gpX.eigen() != X) return 1;
  Eigen::MatrixXd Y(Eigen::MatrixXd::Random(10, 3));
  GPMatrixXd gpSum(X + Y);
  if (gpSum.eigen() != X + Y) return 2; //make sure nothing changes
  GPMatrixXd gpY(Y);
  gpY;
  //gpX + gpY; //type is defined
  // if ((gpX+gpY).eigen() != X+Y) return 3; // + is properly implemented
  // gpSum = gpX + gpY; //type conversion
  return 0;
}
