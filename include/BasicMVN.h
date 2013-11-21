#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>

template <int N = Eigen::Dynamic>
class BasicMVN {

    typedef Eigen::Matrix<double, N, 1> Vec;
    typedef Eigen::Matrix<double, N, N> Mat;

    Vec FX_;
    Vec FM_;
    double lJF_;
    Mat Sigma_;

   public:
    BasicMVN(const Vec& FX, const Vec& FM, double lJF, const Mat& Sigma)
        : FX_(FX), FM_(FM), lJF_(lJF), Sigma_(Sigma) {}

    void set_FX(Vec FX) { FX_ = FX; }
    void set_FM(Vec FM) { FM_ = FM; }
    void set_lJF(double lJF) { lJF_ = lJF; }
    void set_Sigma(Mat Sigma) { Sigma_ = Sigma; }

    double get() const {
        Vec eps(FX_ - FM_);
        Eigen::LDLT<Mat, Eigen::Upper> ldlt(Sigma_);
        double exponent = eps.transpose() * ldlt.solve(eps);
        double lnorm = eps.rows() * std::log(2 * M_PI);
        lnorm += ldlt.vectorD().array().abs().log().sum();
        return 0.5 * (lnorm + exponent);
    }

    Vec get_derivative_FM() const {
        Vec eps(FX_ - FM_);
        Eigen::LDLT<Mat, Eigen::Upper> ldlt(Sigma_);
        return -ldlt.solve(eps);
    }

    Vec get_derivative_FX() const { return -get_derivative_FM(); }

    Mat get_derivative_Sigma() const {
        Vec eps(FX_ - FM_);
        Eigen::LDLT<Mat, Eigen::Upper> ldlt(Sigma_);
        Vec Peps(ldlt.solve(eps));
        Mat Id(Mat::Identity(eps.rows(), eps.rows()));
        Mat P(ldlt.solve(Id));
        return 0.5 * (P - Peps * Peps.transpose());
    }
};
