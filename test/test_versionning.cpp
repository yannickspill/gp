#include "Matrix.h"

#include <Eigen/Dense>

using namespace GP;

int main(int, char * []) {
  const unsigned int szx=10, szy=3;
  Eigen::MatrixXd x(Eigen::MatrixXd::Random(szx,szy));
  Eigen::MatrixXd y(Eigen::MatrixXd::Random(szx,szy));
  GP::MatrixXd vx(x);
  unsigned version = vx.get_version();
  //test get/set
  if (vx.get() != x) return 1;
  vx.set(y);
  if (vx.get() != y) return 2;
  if (vx.get_version() != version+1) return 3;
  //test cloning
  version = vx.get_version();
  GP::MatrixXd vxclone(vx);
  if (version != vxclone.get_version()) return 4;
  vx.set(x);
  version = vx.get_version();
  if (version != vxclone.get_version()) return 5;
  //test versions of branches
  (5*vx).get();
  if ((5*vx).get_version() != version) return 6;
  if ((vx+vx).get_version() != 2*version) return 7;
  if ((vx-vx).get_version() != 2*version) return 8;

  return 0;
}

