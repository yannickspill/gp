#ifndef MVN_H
#define MVN_H

#include "MatrixDifference.h"
#include "LDLT.h"
#include "mvn_MVNEvaluate.h"

#include <Eigen/Dense>

//! Multivariate normal distribution
//FX : F(X)
//lJF : -log(jacobian)
//FM : F(mu)
//Sigma: covariance
template <class VECTORX, class VECTORM, class MATRIX>
class MVN {

    VECTORX FX_;
    VECTORM FM_;
    double lJF_;
    MATRIX Sigma_;

   public:
    MVN(VECTORX FX, VECTORM FM, double lJF, MATRIX Sigma)
        : FX_(FX), FM_(FM), lJF_(lJF), Sigma_(Sigma) {}

    double get() const { return evaluate(); }

   private:
    double evaluate() const {
        //first we transform the inputs to something MVNEvaluate can process
        typedef MatrixDifference<VECTORX, VECTORM> MD;
        MD eps(FX_, FM_);
        LDLT<MATRIX> ldlt(Sigma_);
        MVNEvaluate<MD, LDLT<MATRIX> > eval(eps, ldlt);
        return eval.get() + lJF_;
    }
};

#endif /* MVN_EVALUATE_H */
