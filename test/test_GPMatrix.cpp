#include "Scalar.h"
#include "Matrix.h"

#include <Eigen/Dense>
#include <type_traits>
#include <math.h>

using namespace GP;

int main(int, char * []) {
  // traits classes
  static_assert(std::is_same<MatrixXd::result_type, Eigen::MatrixXd>::value,
                "wrong result_type");
  static_assert(std::is_same<MatrixXd::scalar_type, double>::value,
                "wrong scalar_type");

  // matrix basics
  const unsigned int szx = 100, szy = 30;
  Eigen::MatrixXd x(Eigen::MatrixXd::Random(szx, szy));
  MatrixXd vx(x);
  Eigen::MatrixXd y(Eigen::MatrixXd::Random(szx, szy));
  MatrixXd vy(y);
  if (vx.get() != x) return 1;
  vx.transpose();

  // sum
  vx + vy;
  const Eigen::MatrixXd tmp((vx + vy).get());
  MatrixXd vsum(vy + vx);
  if (vsum.get() != x + y) return 2;
  if ((vx + vy).get() != x + y) return 3;
  internal::MatrixSum<MatrixXd, MatrixXd> s(vx, vy);  // type is defined
  if (s.get() != vsum.get()) return 4;                // works as expected
  // product
  if ((vx.transpose() * vy).get() != x.transpose() * y) return 5;
  // difference
  if ((vx - vy).get() != x - y) return 6;

  // scalar basics
  Scalar scal(3.2);
  if (scal.get() != 3.2) return 7;
  // sum
  if (std::abs((scal + scal).get() - 6.4) > 1e-7) return 8;
  // difference
  if (std::abs((scal - scal).get() - 0.) > 1e-7) return 24;
  // product
  if (std::abs((scal * scal).get() - 10.24) > 1e-7) return 25;
  // quotient
  if (std::abs((scal / scal).get() - 1.) > 1e-7) return 26;

    /*
  // builtin scalar product
  if ((scal * 3.).get() != (scal.get() * 3.)) return 11;
  if ((scal * 3).get() != (scal.get() * 3.)) return 12;
  if ((3. * scal).get() != (scal.get() * 3.)) return 13;
  if ((3 * scal).get() != (scal.get() * 3.)) return 14;
  // builtin scalar quotient
  if ((scal / 3.).get() != (scal.get() / 3.)) return 28;
  if ((3. / scal).get() != (3. / scal.get())) return 29;
  // builtin scalar difference
  if ((scal - 3.).get() != (scal.get() - 3.)) return 30;
  if ((3. - scal).get() != (3. - scal.get())) return 31;
  // builtin scalar sum
  if ((scal + 3.).get() != (scal.get() + 3.)) return 32;
  if ((3. + scal).get() != (3. + scal.get())) return 33;

  // matrix scalar product
  if ((scal * vx).get() != (scal.get() * x)) return 9;
  if ((vx * scal).get() != (scal.get() * x)) return 10;
  // matrix scalar quotient
  if ((vx / scal).get() != (x / scal.get())) return 27;
  // matrix scalar sum (1D)
  MatrixXd small(Eigen::Matrix<double, 1, 1>::Constant(2.));
  if ((small + scal).get() != (scal.get() + small.get()(0, 0))) return 60;
  // matrix scalar difference (1D)
  if ((small - scal).get() != (-scal.get() + small.get()(0, 0))) return 60;
  if ((scal - small).get() != (scal.get() - small.get()(0, 0))) return 61;

  // matrix builtin sum
  if ((5 + small).get() != (5. + small.get()(0, 0))) return 62;
  if ((small + 5.).get() != (5. + small.get()(0, 0))) return 63;
  if (((small + 5.) + 5.).get() != (10. + small.get()(0, 0))) return 66;
  if (((small + 5.) + small).get() != (5. + 2 * small.get()(0, 0))) return 67;
  // matrismall.get()(0,0) builtin difference
  if ((5 - small).get() != (5. - small.get()(0, 0))) return 64;
  if ((small - 5.).get() != (-5. + small.get()(0, 0))) return 65;
  // matrix builtin product
  if ((5 * vx).get() != (5. * x)) return 15;
  if ((vx * 5.).get() != (5. * x)) return 16;
  // matrix builtin quotient
  if ((vx / 5.).get() != (x / 5.)) return 28;

  // matrix and scalar opposites
  if ((-vx).get() != -x) return 69;
  if ((-scal).get() != -(scal.get())) return 70;

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

  */
  // ldlt
  Eigen::MatrixXd L(Eigen::MatrixXd::Random(szy, szy));
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
  if (std::abs(det.get() - std::log(sd.determinant())) > 1e-5) return 21;

  if (std::abs(ldlt.logdet().get() - std::log(sd.determinant())) > 1e-5)
    return 22;

  // solve AX=B
  Eigen::MatrixXd B(Eigen::MatrixXd::Random(szy, szx));
  MatrixXd mB(B);
  Eigen::MatrixXd X(sd.ldlt().solve(B));
  MatrixXd mX(ldlt.solve(mB).get());
  if ((X - mX.get()).array().abs().matrix().norm() > 1e-5) return 23;
  if ((ldlt.solve(mB).get() - sd.ldlt().solve(B)).array().abs().matrix().norm()
      > 1e-5)
    return 24;

  // should be able to perform matrix ops on solve result
  // product
  if (((mB.transpose() * ldlt.solve(mB)).get()
       - (B.transpose() * sd.ldlt().solve(B)))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 50;
  // sum
  if (((mB + ldlt.solve(mB)).get() - (B + sd.ldlt().solve(B)))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 51;
  // difference
  if (((mB - ldlt.solve(mB)).get() - (B - sd.ldlt().solve(B)))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 52;
  // product with scalar
  if (((scal * ldlt.solve(mB)).get() - (scal.get() * sd.ldlt().solve(B)))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 53;
  // product with double
  if (((2 * ldlt.solve(mB)).get() - (2 * sd.ldlt().solve(B)))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 54;
  // quotient with scalar
  if (((ldlt.solve(mB) / scal).get() - (sd.ldlt().solve(B)) / scal.get())
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 55;
  // trace
  if ((ldlt.solve(msd).trace()).get() != (sd.ldlt().solve(sd).trace()))
    return 56;
  // transpose
  if (((ldlt.solve(mB).transpose()).get() - (sd.ldlt().solve(B).transpose()))
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 57;
  // why not decompose it again?!
  if (((msd + ldlt.solve(msd)).decomposition().solve(msd)).get()
      != ((sd + sd.ldlt().solve(sd)).ldlt().solve(sd)))
    return 58;
  if (((msd + ldlt.solve(msd)).decomposition().logdet()).get()
      != ((sd + sd.ldlt().solve(sd))
              .ldlt()
              .vectorD()
              .array()
              .abs()
              .log()
              .sum()))
    return 59;
  /*

  // rows and columns
  MatrixXd dummy(Eigen::MatrixXd::Random(szx, szy));
  if (dummy.rows() != szx) return 26;
  if (dummy.cols() != szy) return 27;

  // trace
  if (dummy.trace().get() != dummy.get().trace()) return 28;

  // ops on determinants and traces should be possible
  double tmp4(
      (msd.decomposition().logdet() * msd.decomposition().logdet()).get());
  double tmp6((msd.trace() + (msd * msd).trace()).get());
  double tmp7((1.0 + 0.5 * msd.decomposition().logdet()).get());

  // multiple products should work too
  if ((msd * 3 * msd).get() != (sd * 3 * sd)) return 29;
  if ((3 * msd * msd).get() != (3 * sd * sd)) return 30;

  // dangling refs causing BAD_ALLOC / segfault (all classes, alphabetical)
  scal.set(5.0);
  Eigen::MatrixXd eId(Eigen::MatrixXd::Identity(szy, szy));
  MatrixXd Id(eId);
  if ((1.0 - (3.0 - (5.0 - scal))).get() != -2.) return 31;
  if ((scal / (1.0 / scal) / scal).get() != 5.0 / (1.0 / 5.0) / 5.0) return 32;
  if ((MatrixXd(msd * Id) * Id).get() != sd * eId * eId) return 33;
  if (((((3 * msd) * 3) * 3).get() - (sd * 27)).array().abs().matrix().norm()
      > 1e-5)
    return 34;
  if (((((msd / 3) / 3) / 3).get() - (sd / 27)).array().abs().matrix().norm()
      > 1e-5)
    return 35;
  if (((msd - Id) - Id - (msd - Id)).get() != sd - eId - eId - (sd - eId))
    return 36;
  if (((msd * Id) * Id).get() != sd * eId * eId) return 37;
  if ((((msd * scal) * msd * (scal * msd)).get() - sd * sd * sd * 25)
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 38;
  if (((((msd / scal) / scal) / scal).get() - sd / 125.)
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 39;
  if (((msd + Id) + Id + (msd + Id)).get() != sd + eId + eId + (sd + eId))
    return 40;
  if ((Scalar(scal * 3) * scal).get() != 75) return 41;
  if ((((scal - 1.0) - 2.0) - 3.0).get() != -1.) return 42;
  if (((2 * (scal * 1.0) * 2.0) * 3.0).get() != 60.) return 43;
  if ((((scal / 1.0) / 2.0) / 3.0).get() != 5. / 6.) return 44;
  if (((2 + (scal + 1.0) + scal) + 3.0).get() != 16.) return 45;
  if (((scal - scal) - scal - (scal - scal)).get() != -5) return 46;
  if (((scal * scal) * scal * (scal * scal)).get() != 3125) return 47;
  if (((scal / scal) / scal / (scal / scal)).get() != 1 / 5.) return 48;
  if (((scal + scal) + scal + (scal + scal)).get() != 25) return 48;
  if (((msd.transpose() + Id * Id.transpose()).get() - sd.transpose()
       - eId * eId.transpose())
          .array()
          .abs()
          .matrix()
          .norm() > 1e-5)
    return 49;

  // broadcast a Scalar to a matrix
  GP::Scalar tbb(0.5);
  auto bcm = GP::MatrixXd::Broadcast(1. / tbb, szx, szy);
  if (bcm.get() != Eigen::MatrixXd::Constant(szx, szy, 2.)) return 71;
  auto bcv = GP::VectorXd::Broadcast(1. / tbb, szx, 1);
  if (bcv.get() != Eigen::VectorXd::Constant(szx, 2.)) return 72;
  if (Eigen::MatrixXd(bcv.asDiagonal().get())
      != Eigen::MatrixXd(2. * Eigen::MatrixXd::Identity(szx, szx)))
    return 73;

  */
  return 0;
}
