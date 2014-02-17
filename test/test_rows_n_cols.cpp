#include "Matrix.h"
#include "Scalar.h"

#include <Eigen/Dense>

#define FAILS(obj, RowsComp, ColsComp, Rows, Cols) \
     ( ((obj).RowsAtCompileTime != RowsComp) \
    || ((obj).ColsAtCompileTime != ColsComp) \
    || ((obj).rows() != Rows) \
    || ((obj).cols() != Cols) \
    || ((obj).cache().RowsAtCompileTime != RowsComp) \
    || ((obj).cache().ColsAtCompileTime != ColsComp) \
    || ((obj).cache().rows() != Rows) \
    || ((obj).cache().cols() != Cols) )


int main(){
    GP::MatrixXd Id(Eigen::MatrixXd::Identity(5,5));
    GP::MatrixXd mxx(Eigen::MatrixXd::Random(5,3));
    GP::VectorXd vx(Eigen::VectorXd::Random(5));
    GP::MatrixXd small(Eigen::MatrixXd::Random(1,1));
    GP::Scalar scal(4.0);
    //builtin*
    if FAILS(1.0-small, 1, 1, 1, 1) return 1;
    if FAILS(1.0-scal, 1, 1, 1, 1) return 2;
    if FAILS(1.0/scal, 1, 1, 1, 1) return 3;
    //scalar*
    if FAILS(scal-4., 1, 1, 1, 1) return 4;
    if FAILS(scal*4., 1, 1, 1, 1) return 5;
    if FAILS(4.*scal, 1, 1, 1, 1) return 6;
    if FAILS(scal/4., 1, 1, 1, 1) return 7;
    if FAILS(scal+4., 1, 1, 1, 1) return 8;
    if FAILS(4.+scal, 1, 1, 1, 1) return 9;
    if FAILS(scal-small, 1, 1, 1, 1) return 10;
    if FAILS(-scal, 1, 1, 1, 1) return 11;
    if FAILS(scal-scal, 1, 1, 1, 1) return 12;
    if FAILS(scal*scal, 1, 1, 1, 1) return 13;
    if FAILS(scal/scal, 1, 1, 1, 1) return 14;
    if FAILS(scal+scal, 1, 1, 1, 1) return 15;
    //matrix*
    if FAILS(mxx, Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 16;
    if FAILS(vx, Eigen::Dynamic, 1, 5, 1) return 17;
    if FAILS(small-1.0, 1, 1, 1, 1) return 18;
    if FAILS(4.*mxx, Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 19;
    if FAILS(mxx*4., Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 20;
    if FAILS(mxx/4., Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 21;
    if FAILS(small+4., 1, 1, 1, 1) return 22;
    if FAILS(4.+small, 1, 1, 1, 1) return 23;
    if FAILS(mxx-mxx, Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 24;
    if FAILS(vx-vx, Eigen::Dynamic, 1, 5, 1) return 25;
    if FAILS(mxx.transpose()*mxx, Eigen::Dynamic, Eigen::Dynamic, 3, 3)
        return 26;
    if FAILS(vx.transpose()*vx, 1, 1, 1, 1) return 27;
    if FAILS(-mxx, Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 28;
    if FAILS(small-scal, 1, 1, 1, 1) return 29;
    if FAILS(vx*scal, Eigen::Dynamic, 1, 5, 1) return 30;
    if FAILS(vx/scal, Eigen::Dynamic, 1, 5, 1) return 31;
    if FAILS(small+scal, 1, 1, 1, 1) return 32;
    if FAILS(scal+small, 1, 1, 1, 1) return 33;
    if FAILS(mxx+mxx, Eigen::Dynamic, Eigen::Dynamic, 5, 3) return 34;
    //ApplyMatrix
    auto f = GP::make_functor(1./scal,scal);
    if FAILS(GP::MatrixXd::Apply(f,vx), Eigen::Dynamic, 1, 5, 1) return 35;
    //LogDet
    if FAILS(Id.decomposition().logdet(), 1, 1, 1, 1) return 36;
    //Solve
    if FAILS(Id.decomposition().solve(vx), Eigen::Dynamic, 1, 5, 1) return 37;
    //Trace
    if FAILS(Id.trace(), 1, 1, 1, 1) return 38;
    //Transpose
    if FAILS(vx.transpose(), 1, Eigen::Dynamic, 1, 5) return 39;
    //scalar exponential
    if FAILS(scal.exp(), 1, 1, 1, 1) return 40;
    //diagonal matrix from vector
    if FAILS(vx.asDiagonal(), Eigen::Dynamic, Eigen::Dynamic, 5, 5) return 41;
    //matrix from scalar
    if FAILS(GP::MatrixXd::Broadcast(scal, 4, 2),
            Eigen::Dynamic, Eigen::Dynamic, 4, 2) return 42;
    if FAILS(GP::VectorXd::Broadcast(scal, 4, 1),
            Eigen::Dynamic, 1, 4, 1) return 43;
    if FAILS(GP::RowVectorXd::Broadcast(scal, 1, 4),
            1, Eigen::Dynamic, 1, 4) return 44;


    return 0;
}
