#include "MVN.h"
#include "FNormal.h"
#include "macros.h"
#include "ConstEigenObject.h"
#include <math.h>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <Eigen/Dense>

namespace {

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXi;

typedef ConstEigenObject<MatrixXd> ConstMat;
typedef ConstEigenObject<VectorXd> ConstVec;
typedef MVN<ConstVec, ConstVec, ConstMat> MultivariateNormal;

typedef boost::mt19937 RNGType;
RNGType generator;

boost::uniform_real<> uniform(0, 1);
boost::variate_generator<RNGType, boost::uniform_real<> > rand(generator,
                                                               uniform);

#define FAIL(str)                      \
    {                                  \
        std::cout << str << std::endl; \
        return false;                  \
    }

#define PRINT(str) std::cout << str << std::endl;

#define RUNTEST(name, nreps)                     \
    for (int i = 0; i < nreps; i++) {            \
        if (!name()) {                           \
            std::cerr << " failed" << std::endl; \
            return 1;                            \
        }                                        \
    }                                            \
    std::cerr << " passed" << std::endl;

#define RUNTEST_N(name, nreps, N)                \
    for (int i = 0; i < nreps; i++) {            \
        if (!name(N)) {                          \
            std::cerr << " failed" << std::endl; \
            return 1;                            \
        }                                        \
    }                                            \
    std::cerr << " passed" << std::endl;

// not are equal, in tolerance.
bool naeq(double a, double b, double delta = 1e-7) {
    if (fabs(b) <= delta) {
        if (fabs(a - b) > delta) {
            std::cout << a << " != " << b << " crit a-b: " << fabs(a - b)
                      << " < " << delta << std::endl;
            return true;
        }
    } else {
        if (a * b < 0 || fabs(fabs((double)a / b) - 1) > delta) {
            std::cout << a << " != " << b
                      << " crit a/b: " << fabs(fabs(a / b) - 1) << " < "
                      << delta << std::endl;
            return true;
        }
    }
    return false;
}

// the distribution should coincide with the normal distribution
// when N=M=1
bool test_degenerate() {
    // observation matrix
    MatrixXd FA(1, 1);
    FA(0, 0) = rand() * 10;
    // Jacobian
    double JA = 1.0;
    double lJA = std::log(JA);
    // mean vector
    VectorXd FM(1);
    FM(0) = rand() * 10;
    // Precision matrix
    MatrixXd Sigma(1, 1);
    Sigma(0, 0) = 1.0 + rand();

    MultivariateNormal mv(ConstVec(FA), ConstVec(FM), lJA, ConstMat(Sigma));
    FNormal fn(FA(0, 0), JA, FM(0), sqrt(Sigma(0, 0)));

    {
        // evaluate
        double observed = mv.get();
        double expected = fn.evaluate();
        if (naeq(observed, expected)) FAIL("evaluate");
    }

    {
        // evaluate_derivative_FM
        double observed = mv.get_derivative_FM()(0);
        double expected = fn.evaluate_derivative_FM();
        if (naeq(observed, expected)) FAIL("evaluate_derivative_FM");
    }

    {
        // evaluate_derivative_Sigma
        double observed = mv.get_derivative_Sigma()(0, 0);
        double expected = fn.evaluate_derivative_sigma();
        expected = expected / (2 * sqrt(Sigma(0, 0)));
        if (naeq(observed, expected)) FAIL("evaluate_derivative_Sigma");
    }
    return true;
}

// the distribution should coincide with two normal distributions
// when N=1 and M=2 in the absence of correlations
bool test_degenerate_N1M2() {
    // observation matrix
    VectorXd FA(2);
    FA(0) = rand() * 10;
    FA(1) = rand() * 10;
    // Jacobian
    double JA = 1.0;
    double lJA = std::log(JA);
    // mean vector
    VectorXd FM(2);
    FM(0) = rand() * 10;
    FM(1) = rand() * 10;
    // Covariance matrix
    MatrixXd Sigma(2, 2);
    Sigma << 1.0 + rand(), 0.0, 0.0, 1.0 + rand();

    ConstVec cvFA(FA);
    ConstVec cvFM(FM);
    ConstMat cmS(Sigma);
    MultivariateNormal mv(cvFA, cvFM, lJA, cmS);
    FNormal fn(FA(0), JA, FM(0), sqrt(Sigma(0, 0)));
    FNormal fn2(FA(1), JA, FM(1), sqrt(Sigma(1, 1)));

    {
        // evaluate
        double observed = mv.get();
        double expected = fn.evaluate() + fn2.evaluate();
        if (naeq(observed, expected)) FAIL("evaluate");
    }

    {
        // evaluate_derivative_FM
        VectorXd observed = mv.get_derivative_FM();
        double expected = fn.evaluate_derivative_FM();
        if (naeq(observed(0), expected)) FAIL("evaluate_derivative_FM 1");
        expected = fn2.evaluate_derivative_FM();
        if (naeq(observed(1), expected)) FAIL("evaluate_derivative_FM 2");
    }

    {
        // evaluate_derivative_Sigma
        MatrixXd observed = mv.get_derivative_Sigma();
        double expected =
            fn.evaluate_derivative_sigma() / (2 * sqrt(Sigma(0, 0)));
        if (naeq(observed(0, 0), expected))
            FAIL("evaluate_derivative_Sigma 1 1");
        expected = fn2.evaluate_derivative_sigma() / (2 * sqrt(Sigma(1, 1)));
        if (naeq(observed(1, 1), expected))
            FAIL("evaluate_derivative_Sigma 2 2");
    }
    return true;
}

// test when M=2 and N=1
bool test_2D() {
    // observation matrix
    VectorXd FA(2);
    FA(0) = 0.5;
    FA(1) = 1.0;
    // FA(1,0)=0.7;
    // FA(1,1)=1.2;
    // Jacobian
    double JA = 1.0;
    double lJA = std::log(JA);
    // mean vector
    VectorXd FM(2);
    FM(0) = 0.0;
    FM(1) = 2.0;
    // Covariance matrix
    MatrixXd Sigma(2, 2);
    double sigma1 = 2.0;
    double sigma2 = 1.0;
    double rho = 0.5;
    Sigma(0, 0) = sigma1 * sigma1;
    Sigma(1, 1) = sigma2 * sigma2;
    Sigma(0, 1) = rho * sigma1 * sigma2;
    Sigma(1, 0) = rho * sigma1 * sigma2;

    MultivariateNormal mv(ConstVec(FA), ConstVec(FM), lJA, ConstMat(Sigma));

    {
        // evaluate
        double observed = mv.get();
        double expected = log(2 * PI * sigma1 * sigma2 * sqrt(1 - rho * rho)) +
                          1 / (2 * (1 - rho * rho)) *
                              (SQUARE(FA(0) - FM(0)) / (sigma1 * sigma1) +
                               SQUARE(FA(1) - FM(1)) / (sigma2 * sigma2) -
                               2 * rho * (FA(0) - FM(0)) *
                                   (FA(1) - FM(1)) / (sigma1 * sigma2));
        if (naeq(observed, expected)) FAIL("evaluate");
    }

    {
        // test_evaluate_derivative_FM
        VectorXd observed = mv.get_derivative_FM();
        double expected =
            ((FM(1) - FA(1)) * rho * sigma1 + (FA(0) - FM(0)) * sigma2) /
            ((-1 + rho * rho) * sigma1 * sigma1 * sigma2);
        if (naeq(observed(0), expected)) FAIL("derivative_FM(0)");
        expected =
            ((FA(1) - FM(1)) * sigma1 + (FM(0) - FA(0)) * rho * sigma2) /
            ((-1 + rho * rho) * sigma1 * sigma2 * sigma2);
        if (naeq(observed(1), expected)) FAIL("derivative_FM(1)");
    }

    {
        // test_evaluate_derivative_Sigma
        double det = Sigma(0, 0) * Sigma(1, 1) - Sigma(0, 1) * Sigma(1, 0);
        double eps1 = FA(0) - FM(0);
        double eps2 = FA(1) - FM(1);
        MatrixXd observed = mv.get_derivative_Sigma();
        double expected = 0.5 / SQUARE(det) *
                          (SQUARE(Sigma(1, 1)) * (Sigma(0, 0) - SQUARE(eps1)) -
                           SQUARE(Sigma(0, 1)) * (Sigma(1, 1) + SQUARE(eps2)) +
                           2 * Sigma(0, 1) * Sigma(1, 1) * eps1 * eps2);
        if (naeq(observed(0, 0), expected)) FAIL("derivative_Sigma(0,0)");
        expected = 0.5 / SQUARE(det) *
                   (SQUARE(Sigma(0, 0)) * (Sigma(1, 1) - SQUARE(eps2)) -
                    SQUARE(Sigma(1, 0)) * (Sigma(0, 0) + SQUARE(eps1)) +
                    2 * Sigma(1, 0) * Sigma(0, 0) * eps2 * eps1);
        if (naeq(observed(1, 1), expected)) FAIL("derivative_Sigma(1,1)");
        expected =
            0.5 / SQUARE(det) *
            (Sigma(0, 1) * (-det + Sigma(1, 1) * SQUARE(eps1) +
                            Sigma(0, 0) * SQUARE(eps2)) -
             eps1 * eps2 * (SQUARE(Sigma(1, 0)) + Sigma(0, 0) * Sigma(1, 1)));
        if (naeq(observed(0, 1), expected)) FAIL("derivative_Sigma(0,1)");
        if (naeq(observed(1, 0), expected)) FAIL("derivative_Sigma(1,0)");
    }
    return true;
}
}

int main(int, char * []) {
    PRINT("degenerate N=1 M=1");
    RUNTEST(test_degenerate, 100);
    PRINT("degenerate N=1 M=2");
    RUNTEST(test_degenerate_N1M2, 100);
    PRINT("2D");
    RUNTEST(test_2D, 100);
    return 0;
}
