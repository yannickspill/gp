#include "Matrix.h"
#include "Scalar.h"
#include "internal/Cache.h"

#include <Eigen/Dense>
#include <iostream>

namespace{
template <class Expr, class CExpr>
bool get_check(const Expr& expr, const CExpr& cached, std::true_type) {
  return expr.get().eval() != cached.get();
}

template <class Expr, class CExpr>
bool get_check(const Expr& expr, const CExpr& cached, std::false_type) {
    //return expr.get() != cached.get();
    expr.get(), cached.get(); //update internal cache
    return false; //operator!= not always defined, skip test.
}

template <class Expr, class CExpr>
bool cache_disconnected(const Expr& expr, const CExpr& cached) {
  if (get_check(expr,cached, typename CExpr::eval_called())) return true;
  if (expr.get_version() != cached.get_version()) return true;
  return false;
}

template <class Expr, class Mat, class Val>
bool cache_fails(const Expr& expr, Mat& mat, const Val& val1, const Val& val2) {
  auto cexpr = expr.cache();
  mat.set(val1);
  if (cache_disconnected(expr, cexpr)) return true;
  unsigned version = expr.get_version();
  mat.set(val2);
  if (cache_disconnected(expr, cexpr)) return true;
  if (cexpr.get_version() == version) return true;
  return false;
}
}

int main(int, char * []) {
  {
    // manual instantiation
    GP::MatrixXd mat(Eigen::MatrixXd::Constant(5, 3, 2.0));
    GP::internal::Cache<GP::MatrixXd> cmat(mat);
    if (cache_disconnected(mat, cmat)) return 1;
    unsigned version = mat.get_version();
    mat.set(Eigen::MatrixXd::Random(4, 3));
    if (cache_disconnected(mat, cmat)) return 2;
    if (cmat.get_version() == version) return 3;
  }
  {
    // instantiation through member
    GP::MatrixXd mat(Eigen::MatrixXd::Constant(5, 3, 2.0));
    auto cmat(mat.cache());
    if (cache_disconnected(mat, cmat)) return 4;
    unsigned version = mat.get_version();
    mat.set(Eigen::MatrixXd::Random(4, 3));
    if (cache_disconnected(mat, cmat)) return 5;
    if (cmat.get_version() == version) return 6;
  }
  {
    // member instantiation
    GP::MatrixXd mat(Eigen::MatrixXd::Identity(5, 5));
    GP::VectorXd vec(Eigen::VectorXd::LinSpaced(5, 2, 3));
    auto expr = mat * vec;
    auto cexpr = expr.cache();
    if (cache_disconnected(expr, cexpr)) return 8;
    unsigned version = expr.get_version();
    vec.set(Eigen::VectorXd::LinSpaced(5, 5, 12));
    if (cache_disconnected(expr, cexpr)) return 9;
    if (cexpr.get_version() == version) return 10;
  }
  // see if cache works on various expressions
  // mat*vec
  GP::MatrixXd mat(Eigen::MatrixXd::Identity(5, 5));
  GP::VectorXd vec(Eigen::VectorXd::LinSpaced(5, 2, 3));
  if (cache_fails(mat * vec, vec, Eigen::VectorXd::LinSpaced(5, 2, 3),
                  Eigen::VectorXd::LinSpaced(5, -2., 10)))
    return 7;
  // vec^T * mat * vec
  if (cache_fails(vec.transpose() * mat * vec, vec,
                  Eigen::VectorXd::LinSpaced(5, 2, 3),
                  Eigen::VectorXd::LinSpaced(5, -2., 10)))
    return 11;
  // ldlt(mat)
  if (cache_fails(mat.decomposition(), mat,
                  3*Eigen::MatrixXd::Identity(5, 5),
                  2*Eigen::MatrixXd::Identity(5, 5)))
    return 12;
  // logdet(mat)
  if (cache_fails(mat.decomposition().logdet(), mat,
                  3*Eigen::MatrixXd::Identity(5, 5),
                  2*Eigen::MatrixXd::Identity(5, 5)))
    return 13;
  // ldlt(mat*mat)
  if (cache_fails((mat*mat).decomposition(), mat,
                  3*Eigen::MatrixXd::Identity(5, 5),
                  2*Eigen::MatrixXd::Identity(5, 5)))
    return 14;
  // solve: does not work. Need to cast to matrix first.
  /*
  if (cache_fails((mat*mat).decomposition().solve(vec), vec,
                  Eigen::VectorXd::LinSpaced(5, 2, 3),
                  Eigen::VectorXd::LinSpaced(5, -2., 10)))
    return 15;
    */
  // vec^T * mat * vec
  if (cache_fails(vec.transpose(), vec,
                  Eigen::VectorXd::LinSpaced(5, 2, 3),
                  Eigen::VectorXd::LinSpaced(5, -2., 10)))
    return 16;

  //things involving a Scalar
  GP::Scalar s(3.0);
  if (cache_fails(s, s, 1.0, 2.0)) return 17;
  if (cache_fails(s+2*s, s, 1.0, 2.0)) return 18;
  if (cache_fails(s*mat, s, 1.0, 2.0)) return 19;


  return 0;
}
