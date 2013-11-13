#ifndef FNORMAL_H
#define FNORMAL_H

#include "macros.h"
#include <math.h>

//! FNormal
/** Probability density function and -log(p) of normal sampling from some
 * function F. If A is drawn from the F-Normal distribution then F(A) is drawn
 * from a normal distribution with mean M and standard deviation sigma.
 * Arguments: F(A), J(A) the derivative of F w/r to A, F(M) and sigma.  The
 * distribution is normalized with respect to the variable A.
 *
 *  Example: if F is the log function, the F-normal distribution is the
 *  lognormal distribution with mean M and standard deviation sigma.
 *
 *  NOTE: for now, F must be monotonically increasing, so that JA > 0. The
 *  program will not check for that.
 */

class FNormal
{
 public:
  FNormal(double FA, double JA, double FM, double sigma) :
    FA_(FA),
    JA_(JA),
    FM_(FM),
    sigma_(sigma) {}

  /* energy (score) functions, aka -log(p) */
  virtual double evaluate() const
  {
      return -log(JA_/sigma_) + 0.5*log(2*PI)
            + 1/(2*SQUARE(sigma_))*SQUARE(FA_-FM_);
  }

  //derivative of score wrt F(A)
  virtual double evaluate_derivative_FA() const
  { return (FA_-FM_)/SQUARE(sigma_); }

  virtual double evaluate_derivative_JA() const
  { return -1/JA_; }

  //derivative wrt F(M)
  virtual double evaluate_derivative_FM() const
  { return (FM_-FA_)/SQUARE(sigma_); }

  virtual double evaluate_derivative_sigma() const
  { return 1/sigma_ - SQUARE(FA_-FM_)/pow(sigma_,3); }

  /* probability density function */
  virtual double density() const
  {
      return JA_/(sqrt(2*PI)*sigma_)*
          exp(-SQUARE(FA_-FM_)/(2*SQUARE(sigma_)));
  }

  /* change of parameters */
  void set_FA(double f) {
    FA_=f;
  }
  void set_JA(double f) {
    JA_=f;
  }
  void set_FM(double f) {
    FM_=f;
  }
  void set_sigma(double f) {
    sigma_=f;
  }

 private:
  double FA_,JA_,FM_,sigma_;
};

#endif  /* FNORMAL_H */
