#include "internal/Matrix.h"
#include "internal/functors.h"
#include "internal/BinaryOp.h"
#include "internal/MatrixDifference.h"
#include "internal/MatrixMatrixProduct.h"
#include "internal/Scalar.h"
#include "internal/ScalarScalarProduct.h"
#include "internal/ScalarBuiltinProduct.h"
#include "internal/MatrixScalarProduct.h"
#include "internal/MatrixBuiltinProduct.h"
#include "internal/Transpose.h"
#include "internal/LDLTPolicy.h"
#include "internal/Decomposition.h"
#include "internal/LogDeterminant.h"
#include "internal/Solve.h"

#include <Eigen/Dense>
#include <type_traits>
#include <math.h>

using namespace GP::internal;

int main(int, char * []) {
  // traits classes
  static_assert(std::is_same<MatrixXd::result_type, Eigen::MatrixXd>::value,
                "wrong result_type");
  static_assert(std::is_same<MatrixXd::scalar_type, double>::value,
                "wrong scalar_type");

  // matrix basics
  const unsigned int szx = 10, szy = 3;
  Eigen::MatrixXd x(Eigen::MatrixXd::Constant(szx, szy, 1));
  MatrixXd vx(x);
  Eigen::MatrixXd y(Eigen::MatrixXd::Constant(szx, szy, 2));
  MatrixXd vy(y);
  if (vx.get() != x) return 1;
  // sum
  vx + vy;
  const Eigen::MatrixXd tmp((vx + vy).get());
  MatrixXd vsum(vy + vx);
  if (vsum.get() != Eigen::MatrixXd::Constant(szx, szy, 3)) return 2;
  if ((vx + vy).get() != Eigen::MatrixXd::Constant(szx, szy, 3)) return 3;
  GP::internal::BinaryOp
      <MatrixXd, MatrixXd, sum_binary_op> s(vx, vy);  // type is defined
  if (s.get() != vsum.get()) return 4;  // works as expected
  // product
  if ((vx.transpose() * vx).get() != x.transpose() * x) return 5;
  // difference
  if ((vx - vy).get() != Eigen::MatrixXd::Constant(szx, szy, -1)) return 6;

  // scalar basics
  GP::internal::Scalar scal(3.2);
  if (scal.get() != 3.2) return 7;
  // sum
  if (std::abs((scal + scal).get() - 6.4) > 1e-7) return 25;
  // difference
  if (std::abs((scal - scal).get() - 0.) > 1e-7) return 26;
  // product
  if (std::abs((scal * scal).get() - 10.24) > 1e-7) return 27;
  // quotient
  if (std::abs((scal / scal).get() - 1.) > 1e-7) return 28;

  // builtin scalar product
  if ((scal * 3.).get() != (scal.get() * 3.)) return 11;
  if ((scal * 3).get() != (scal.get() * 3.)) return 12;
  if ((3. * scal).get() != (scal.get() * 3.)) return 13;
  if ((3 * scal).get() != (scal.get() * 3.)) return 14;

  // matrix scalar product
  if ((scal * vx).get() != (scal.get() * x)) return 9;
  if ((vx * scal).get() != (scal.get() * x)) return 10;

  // matrix builtin product
  if ((5 * vx).get() != (5. * x)) return 15;
  if ((vx * 5.).get() != (5. * x)) return 16;

  // lots of products sums and divisions
  if ((vx.transpose() * (5 * vx + vy - scal * vx)).get()
      != (x.transpose() * (5 * x + y - scal.get() * x)))
    return 17;

  // transpose
  if (vx.transpose().get() != x.transpose()) return 17;
  if ((vx + vx).transpose().get() != (x + x).transpose()) return 18;
  if ((vx - vx).transpose().get() != (x - x).transpose()) return 19;
  if ((vx - scal * vx).transpose().get() != (x - scal.get() * x).transpose())
    return 20;

  // ldlt
  Eigen::MatrixXd L(Eigen::MatrixXd::Random(5, 5));
  L.diagonal() = L.diagonal().array().abs();
  L = L.triangularView<Eigen::Lower>();
  Eigen::MatrixXd sd(L * L.transpose());
  MatrixXd msd(sd);
  auto ldlt = msd.decomposition();
  ldlt.get();
  (sd * sd).ldlt();
  (msd * msd).decomposition();
  (msd * msd).decomposition().get();

  // determinant
  LogDeterminant<decltype(ldlt)> det(ldlt);
  static_assert(std::is_same<decltype(det), decltype(ldlt.logdet())>::value,
                "MatrixBase passes wrong type to LogDeterminant");
  if (std::abs(det.get() - std::log(sd.determinant())) > 1e-7) return 21;

  if (std::abs(ldlt.logdet().get() - std::log(sd.determinant())) > 1e-7)
    return 22;

  // solve AX=B
  Eigen::MatrixXd B(Eigen::MatrixXd::Random(5, 2));
  MatrixXd mB(B);
  Eigen::MatrixXd X(sd.ldlt().solve(B));
  MatrixXd mX(ldlt.solve(mB).get());
  if ((X - mX.get()).array().abs().matrix().norm() > 1e-5) return 23;
  if ((ldlt.solve(mB).get() - sd.ldlt().solve(B)).array().abs().matrix().norm()
      > 1e-5)
    return 24;
  return 0;
}
