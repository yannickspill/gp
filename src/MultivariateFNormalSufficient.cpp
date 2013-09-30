/**
 *  \file MultivariateFNormalSufficient.cpp
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 */

#include "MultivariateFNormalSufficient.h"

MultivariateFNormalSufficient::MultivariateFNormalSufficient(
        const MatrixXd& FX, double JF, const VectorXd& FM,
        const MatrixXd& Sigma, double factor)
{
        //O(1)
        reset_flags();
        N_=FX.rows();
        M_=FX.cols();
        LOG( "MVN: direct init with N=" << N_
                << " and M=" << M_ << std::endl);
        CHECK( N_ > 0,
            "please provide at least one observation per dimension");
        CHECK( M_ > 0,
            "please provide at least one variable");
        set_factor(factor);
        set_FM(FM);
        set_FX(FX);
        set_jacobian(JF);
        set_Sigma(Sigma);
}

MultivariateFNormalSufficient::MultivariateFNormalSufficient(
        const VectorXd& Fbar, double JF, const VectorXd& FM, int Nobs,
        const MatrixXd& W, const MatrixXd& Sigma, double factor)
{
        reset_flags();
        N_=Nobs;
        M_=Fbar.rows();
        LOG( "MVN: sufficient statistics init with N=" << N_
                << " and M=" << M_ << std::endl);
        CHECK( N_ > 0,
            "please provide at least one observation per dimension");
        CHECK( M_ > 0,
            "please provide at least one variable");
        set_factor(factor);
        set_FM(FM);
        set_Fbar(Fbar);
        set_W(W);
        set_jacobian(JF);
        set_Sigma(Sigma);
}

void MultivariateFNormalSufficient::reset_flags()
{
  flag_FM_ = false;
  flag_FX_ = false;
  flag_Fbar_ = false;
  flag_W_ = false;
  flag_Sigma_ = false;
  flag_epsilon_ = false;
  flag_PW_ = false;
  flag_P_ = false;
  flag_ldlt_ = false;
  flag_norms_ = false;
  flag_Peps_ = false;
}

  /* probability density function */
double MultivariateFNormalSufficient::density() const
  {
      double d;
      if (N_ == 1)
      {
          d=get_norms()[0]*get_jacobian()
              *exp(-0.5*get_mean_square_residuals()/SQUARE(factor_));
      } else {
          d = get_norms()[0]*get_jacobian()
          *exp(-0.5*(trace_WP()
                     + N_ * get_mean_square_residuals())/SQUARE(factor_));
      }
      LOG( "MVN: density() = " << d << std::endl);
      return d;
  }

  /* energy (score) functions, aka -log(p).
   * O(M) if Sigma is up to date
   * O(M^3) if Sigma has changed (Cholesky decomp.) */
double MultivariateFNormalSufficient::evaluate() const
  {
      double e = get_norms()[1] + get_minus_log_jacobian();
      if (N_==1)
      {
          e += 0.5*get_mean_square_residuals()/SQUARE(factor_) ;
          /*std::cout << "mvn"
              << " " << e
              << " " << get_norms()[0]
              << " " << get_norms()[1]
              << " " << get_minus_log_jacobian()
              << " " << get_mean_square_residuals()/SQUARE(factor_)
              <<std::endl; */
      } else {
          e += 0.5*( trace_WP()
                + double(N_)*get_mean_square_residuals())/SQUARE(factor_) ;
      }
      //if log(norm) is -inf, make the score bad, e.g. +inf.
      //should check if epsilon==0 in which case we should return -inf
      //to mimic the Dirac distribution, but this case has undefined derivatives
      if (std::abs(e) > std::numeric_limits<double>::max())
          e=std::numeric_limits<double>::infinity();
      LOG( "MVN: evaluate() = " << e << std::endl);
      return e;
  }

double MultivariateFNormalSufficient::get_minus_exponent() const
{
    double e;
    if (N_ == 1)
    {
        e = 0.5*get_mean_square_residuals()/SQUARE(factor_) ;
    } else {
        e = 0.5*( trace_WP()
                + double(N_)*get_mean_square_residuals())/SQUARE(factor_) ;
    }
    return e;
}

double MultivariateFNormalSufficient::get_minus_log_normalization() const
{
    double e = get_norms()[1] + get_minus_log_jacobian();
    return e;
}


  // O(1) if up to date, O(M^2) if epsilon new, O(M^3) if Sigma new
VectorXd MultivariateFNormalSufficient::evaluate_derivative_FM() const
{
      // d(-log(p))/d(FM) = - N * P * epsilon
      LOG( "MVN: evaluate_derivative_FM() = " << std::endl);
      VectorXd tmp(-N_ * get_Peps()/SQUARE(factor_));
      return tmp;
}

  MatrixXd MultivariateFNormalSufficient::evaluate_derivative_Sigma() const
  {
      //d(-log(p))/dSigma = 1/2 (N P - N P epsilon transpose(epsilon) P - PWP)
      LOG( "MVN: evaluate_derivative_Sigma() = " << std::endl);
      MatrixXd R;
      if (N_==1) // O(M) if up to date, O(M^3) if Sigma new
      {
          R = 0.5*(get_P()-compute_PTP()/SQUARE(factor_));
      } else { // O(M^2) if up to date, O(M^3) if Sigma new
          double f2=SQUARE(factor_);
          R = 0.5*(N_*(get_P()-compute_PTP()/f2)-compute_PWP()/f2);
      }
      return R;
  }

  double MultivariateFNormalSufficient::evaluate_derivative_factor() const
  {
      //warning: untested!
      //d(-log(p))/dfactor = -N/f^3 trans(eps)P(eps) -1/f^3 tr(WP) + NM/f
      LOG( "MVN: evaluate_derivative_factor() = " << std::endl);
      double R;
      if (N_==1)
      {
          R = - get_mean_square_residuals()/CUBE(factor_)
              + double(M_)/factor_;
      } else {
          R = - (double(N_)*get_mean_square_residuals()
                  + trace_WP())/CUBE(factor_)
              + double(M_*N_)/factor_;
      }
      return R;
  }

  MatrixXd MultivariateFNormalSufficient::evaluate_second_derivative_FM_FM()
    const
  {
      CHECK(N_==1, "not implemented when N>1");
      MatrixXd ret(get_P()/SQUARE(factor_));
      return ret;
  }

  MatrixXd MultivariateFNormalSufficient::evaluate_second_derivative_FM_Sigma(
          unsigned k) const
  {
      CHECK(N_==1, "not implemented when N>1");
      MatrixXd P(get_P());
      VectorXd Peps(get_Peps());
      MatrixXd ret(P.transpose().col(k)*Peps.transpose());
      return ret/SQUARE(factor_);
  }

MatrixXd MultivariateFNormalSufficient::evaluate_second_derivative_Sigma_Sigma(
        unsigned m, unsigned n) const
  {
      CHECK(N_==1, "not implemented when N>1");
      MatrixXd P(get_P());
      VectorXd Peps(get_Peps());
      MatrixXd tmp(P.col(m)*Peps.transpose());
      MatrixXd ret(0.5*(-P.col(n)*P.row(m)
                        +Peps(n)*(tmp+tmp.transpose())));
      return ret/SQUARE(factor_);
  }

  MatrixXd MultivariateFNormalSufficient::get_FX() const
{
    CHECK(flag_FX_, "FX was not set!");
    return FX_;
}

  void MultivariateFNormalSufficient::set_FX(const MatrixXd& FX)
  {
    if (FX.rows() != FX_.rows() || FX.cols() != FX_.cols() || FX != FX_){
        CHECK(FX.rows() == N_,
                "size mismatch for FX in the number of repetitions: got "
                    << FX.rows() << " instead of "<<N_);
        CHECK(FX.cols() == M_,
                "size mismatch for FX in the number of variables: got "
                    <<FX.cols() << " instead of "<<M_);
        FX_=FX;
        LOG( "MVN:   set FX to new matrix"<< std::endl);
        flag_Fbar_ = false;
        flag_epsilon_ = false;
        flag_W_ = false;
        flag_PW_ = false;
        flag_Peps_ = false;
    }
    flag_FX_ = true;
  }

  VectorXd MultivariateFNormalSufficient::get_FM() const
{
    CHECK(flag_FM_, "FM was not set!");
    return FM_;
}

  void MultivariateFNormalSufficient::set_FM(const VectorXd& FM)
  {
    if (FM.rows() != FM_.rows() || FM.cols() != FM_.cols() || FM != FM_){
        CHECK(FM.rows() == M_,
            "size mismatch for FM: got "
            <<FM.rows() << " instead of " << M_);
        FM_=FM;
        LOG( "MVN:   set FM to new vector" << std::endl);
        flag_epsilon_ = false;
        flag_Peps_ = false;
    }
    flag_FM_ = true;
  }

  VectorXd MultivariateFNormalSufficient::get_Fbar() const
{
    if (!flag_Fbar_)
    {
        // Fbar is either given or computed from FX
        // try to get FX which will throw an exception if it is not set
        VectorXd Fbar = get_FX().colwise().mean();
        const_cast<MultivariateFNormalSufficient *>(this)->set_Fbar(Fbar);
    }
    return Fbar_;
}

  void MultivariateFNormalSufficient::set_Fbar(const VectorXd& Fbar)
  {
    if (Fbar.rows() != Fbar_.rows() || Fbar.cols() != Fbar_.cols()
            || Fbar != Fbar_){
        CHECK(Fbar.rows() == M_,
            "size mismatch for Fbar: got "
                    << Fbar.rows() << " instead of " << M_);
        Fbar_=Fbar;
        LOG( "MVN:   set Fbar to new vector" << std::endl);
        flag_epsilon_ = false;
        flag_W_ = false;
        flag_PW_ = false;
        flag_Peps_ = false;
    }
    flag_Fbar_ = true;
  }

  double MultivariateFNormalSufficient::get_jacobian() const
{
    return JF_;
}

  void MultivariateFNormalSufficient::set_jacobian(double JF)
  {
        JF_=JF;
        lJF_=-log(JF_);
  }

  double MultivariateFNormalSufficient::get_minus_log_jacobian() const
{
    return lJF_;
}

  void MultivariateFNormalSufficient::set_minus_log_jacobian(double lJF)
  {
        JF_=exp(-lJF);
        lJF_=lJF;
  }

  VectorXd MultivariateFNormalSufficient::get_epsilon() const
{
    if (!flag_epsilon_)
    {
        VectorXd epsilon = get_Fbar() - get_FM();
        const_cast<MultivariateFNormalSufficient*>(this)->set_epsilon(epsilon);
    }
    return epsilon_;
}

  void MultivariateFNormalSufficient::set_epsilon(const VectorXd& eps)
{
    LOG( "MVN:   set epsilon to new vector"<< std::endl);
    epsilon_ = eps;
    flag_epsilon_ = true;
    flag_Peps_ = false;
}

  MatrixXd MultivariateFNormalSufficient::get_W() const
{
    if (!flag_W_)
    {
        //this only happens if W was not given as input
        //compute W from epsilon
        LOG( "MVN:   computing W" << std::endl);
        MatrixXd W(M_,M_);
        if (N_ == 1)
        {
            W.setZero();
        } else {
            MatrixXd A(get_FX().rowwise() - get_Fbar().transpose());
            W = A.transpose()*A;
        }
        const_cast<MultivariateFNormalSufficient *>(this)->set_W(W);
    }
    return W_;
}

  void MultivariateFNormalSufficient::set_W(const MatrixXd& W)
{
    if (W.rows() != W_.rows() || W.cols() != W_.cols() || W != W_)
    {
        CHECK(W.cols() == W.rows(), "need a square matrix!");
        CHECK(W.rows() == M_, "size mismatch for W!");
        W_ = W;
        LOG( "MVN:   set W to new matrix"<< std::endl);
        flag_PW_ = false;
    }
    flag_W_ = true;
}

  MatrixXd MultivariateFNormalSufficient::get_Sigma() const
{
    CHECK(flag_Sigma_, "Sigma was not set!");
    return Sigma_;
}

  void MultivariateFNormalSufficient::set_Sigma(const MatrixXd& Sigma)
  {
    if (Sigma.rows() != Sigma_.rows() || Sigma.cols() != Sigma_.cols()
            || Sigma != Sigma_){
        CHECK(Sigma.cols() == Sigma.rows(), "need a square matrix!");
        Sigma_=Sigma;
        //std::cout << "Sigma: " << Sigma_ << std::endl << std::endl;
        LOG( "MVN:   set Sigma to new matrix" << std::endl);
        flag_ldlt_ = false;
        flag_P_ = false;
        flag_PW_ = false;
        flag_norms_ = false;
        flag_Peps_ = false;
    }
    flag_Sigma_ = true;
  }

  double MultivariateFNormalSufficient::get_factor() const
{
    return factor_;
}

  void MultivariateFNormalSufficient::set_factor(double f)
{
    factor_=f;
    flag_norms_ = false;
}

//Eigen::LLT<MatrixXd, Eigen::Upper>
Eigen::LDLT<MatrixXd, Eigen::Upper>
MultivariateFNormalSufficient::get_ldlt() const
{
    if (!flag_ldlt_)
    {
        LOG( "MVN:   computing Cholesky decomposition" << std::endl);
        // compute Cholesky decomposition for determinant and inverse
        //Eigen::LLT<MatrixXd, Eigen::Upper> ldlt(get_Sigma());
        Eigen::LDLT<MatrixXd, Eigen::Upper> ldlt(get_Sigma());
        //if (ldlt.info() != Eigen::Success)
        CHECK(ldlt.isPositive(),
            "Sigma matrix is not positive semidefinite!");
        const_cast<MultivariateFNormalSufficient *>(this)->set_ldlt(ldlt);
    }
    return ldlt_;
}

void MultivariateFNormalSufficient::set_ldlt(
          const Eigen::LDLT<MatrixXd, Eigen::Upper>& ldlt)
{
    LOG( "MVN:   set ldlt factorization"<< std::endl);
    ldlt_ = ldlt;
    flag_ldlt_ = true;
    flag_P_ = false;
    flag_PW_ = false;
    flag_norms_ = false;
    flag_Peps_ = false;
}

std::vector<double> MultivariateFNormalSufficient::get_norms() const
{
    if (!flag_norms_)
    {
        //Eigen::LLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
        Eigen::LDLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
        // determinant and derived constants
        //MatrixXd L(ldlt.matrixU());
        //std::cout << "L: " << L << std::endl << std::endl;
        //std::cout << "D: " << ldlt.vectorD() << std::endl << std::endl;
        //double logDetSigma=2*L.diagonal().array().log().sum() ;
        double logDetSigma = ldlt.vectorD().array().abs().log().sum();
        LOG( "MVN:   det(Sigma) = " <<exp(logDetSigma) << std::endl);
        double norm=pow(2*PI*SQUARE(factor_), -double(N_*M_)/2.0)
                    * exp(-double(N_)/2.0*logDetSigma);
        double lnorm=double(N_*M_)/2 * log(2*PI*SQUARE(factor_))
            + double(N_)/2 * logDetSigma;
        LOG( "MVN:   norm = " << norm << " lnorm = "
                << lnorm << std::endl);
        const_cast<MultivariateFNormalSufficient *>(this)
            ->set_norms(norm,lnorm);
    }
    std::vector<double> norms;
    norms.push_back(norm_);
    norms.push_back(lnorm_);
    return norms;
}

void MultivariateFNormalSufficient::set_norms(double norm, double lnorm)
{
    norm_ = norm;
    lnorm_ = lnorm;
    LOG( "MVN:   set norms" << std::endl);
    flag_norms_ = true;
}

MatrixXd MultivariateFNormalSufficient::get_P() const
{
    if (!flag_P_)
    {
        //inverse
        //Eigen::LLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
        Eigen::LDLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
        LOG( "MVN:   solving for inverse" << std::endl);
        const_cast<MultivariateFNormalSufficient *>(this)
            ->set_P(ldlt.solve(MatrixXd::Identity(M_,M_)));
    }
    return P_;
}

void MultivariateFNormalSufficient::set_P(const MatrixXd& P)
{
    P_ = P;
    //std::cout << "P: " << P_ << std::endl << std::endl;
    LOG( "MVN:   set P to new matrix" << std::endl);
    flag_P_ = true;
}

MatrixXd MultivariateFNormalSufficient::get_PW() const
{
    if (!flag_PW_)
    {
        ////PW
        MatrixXd PW(M_,M_);
        if (N_==1)
        {
            LOG( "MVN:   W=0 => PW=0" << std::endl);
            PW.setZero();
        } else {
            LOG( "MVN:   solving for PW" << std::endl);
            PW = compute_PW_direct();
        }
        const_cast<MultivariateFNormalSufficient *>(this)->set_PW(PW);
    }
    return PW_;
}

MatrixXd MultivariateFNormalSufficient::compute_PW_direct() const
{
    //Eigen::LLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
    Eigen::LDLT<MatrixXd, Eigen::Upper> ldlt(get_ldlt());
    MatrixXd tmp(ldlt.solve(get_W()));
    return tmp;
}

void MultivariateFNormalSufficient::set_PW(const MatrixXd& PW)
{
    PW_ = PW;
    //std::cout << "PW: " << PW_ << std::endl << std::endl;
    LOG( "MVN:   set PW to new matrix" << std::endl);
    flag_PW_ = true;
}

VectorXd MultivariateFNormalSufficient::get_Peps() const
{
    if (!flag_Peps_)
    {
        ////Peps
        LOG( "MVN:   solving for P*epsilon" << std::endl);
        const_cast<MultivariateFNormalSufficient *>(this)
            ->set_Peps(get_ldlt().solve(get_epsilon()));
    }
    return Peps_;
}

void MultivariateFNormalSufficient::set_Peps(const VectorXd& Peps)
{
    Peps_ = Peps;
    //std::cout << "Peps: " << Peps_ << std::endl << std::endl;
    LOG( "MVN:   set P*epsilon to new matrix" << std::endl);
    flag_Peps_ = true;
}

  double MultivariateFNormalSufficient::trace_WP() const
  {
      double trace;
      if (N_==1)
      {
          trace=0;
      } else {
          trace = (get_P()*get_W()).trace();
          LOG( "MVN:   trace(WP) = " << trace << std::endl);
      }
      return trace;
  }

  double MultivariateFNormalSufficient::get_mean_square_residuals() const
{
    //std::cout << "P " << std::endl << P_ << std::endl;
    //std::cout << "epsilon " << std::endl << get_epsilon() << std::endl;
    VectorXd Peps(get_Peps());
    VectorXd epsilon(get_epsilon());
    double dist = epsilon.transpose()*Peps;
    LOG( "MVN:   get_mean_square_residuals = " << dist << std::endl);
    return dist;
}

  MatrixXd MultivariateFNormalSufficient::compute_PTP() const
{
  LOG( "MVN:   computing PTP" << std::endl);
  VectorXd peps(get_Peps());
  MatrixXd tmp(peps*peps.transpose());
  return tmp;
}

MatrixXd MultivariateFNormalSufficient::compute_PWP() const
{
      //compute PWP
      MatrixXd tmp(M_,M_);
      if (N_==1)
      {
          LOG( "MVN:   W = 0 => PWP = 0" << std::endl);
          tmp.setZero();
      } else {
          LOG( "MVN:   computing PWP" << std::endl);
          MatrixXd P(get_P());
          MatrixXd W(get_W());
          tmp=P*W*P;
      }
      return tmp;
}

VectorXd MultivariateFNormalSufficient::get_Sigma_eigenvalues() const
{
    Eigen::SelfAdjointEigenSolver<MatrixXd> eigensolver(get_Sigma(),
            Eigen::EigenvaluesOnly);
    CHECK(eigensolver.info() == Eigen::Success,
            "Could not determine eigenvalues!");
    return eigensolver.eigenvalues();
}

double MultivariateFNormalSufficient::get_Sigma_condition_number() const
{
    return get_Sigma().norm()*get_P().norm();
}

MatrixXd MultivariateFNormalSufficient::solve(MatrixXd B) const
{
    return get_ldlt().solve(B);
}
