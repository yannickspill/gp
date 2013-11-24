#include "benchmark_helpers.h"
#include "macros.h"
#include "MVN.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>

using boost::posix_time::seconds;
using boost::posix_time::microsec_clock;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

typedef Eigen::VectorXd EigenVec;
typedef Eigen::MatrixXd EigenMat;
typedef ConstEigenObject<EigenVec> Vec;
typedef ConstEigenObject<EigenMat> Mat;
typedef MVN<Vec, Vec, Mat> Multivariate;

// Examine the scaling of MVN::get() when called for the first time
int main(int, char*[]){
    //dimensions which will be tried
    const std::vector<unsigned> ndims {2,3,5,8,10,
        12,15,18,20,25,30,40,50,60,70,80,90,100,150,200,250,300,350,400,450,
        500,600,700,800,900,1000,1100,1200,1500,2000};
    //maximum time spent on a single dimension
    const time_duration maxtime=seconds(1);
    //minimum number of calls to make for a single dimension
    const unsigned mincalls=10;
    //
    std::cout << "ndims runtime(us) : MVN constructor" << std::endl;
    for (unsigned n : ndims){
        //Build inputs
        Vec y(EigenVec::Constant(n, 1.0));
        Vec m(EigenVec::LinSpaced(n,0,1));
        EigenVec x(EigenVec::LinSpaced(n,0,1));
        std::vector<double> params(NUM_PARAMS, 0.);
        params[PARAM_TAU] = 1.;
        params[PARAM_LAMBDA] = 0.1;
        Mat Sigma(get_prior_covariance(x, params, 0.01));
        //perform benchmark
        std::vector<time_duration> thisrun;
        unsigned ncalls = 0;
        ptime run = microsec_clock::local_time();
        do{
            Multivariate mvn(y,m,0.0,Sigma);
            //
            // BENCHMARK
            ptime start = microsec_clock::local_time();
            mvn.get();
            mvn.get_derivative_FM();
            mvn.get_derivative_Sigma();
            ptime end = microsec_clock::local_time();
            //
            //
            thisrun.push_back(end-start);
        } while ((++ncalls < mincalls) ||
                 (microsec_clock::local_time() - run < maxtime));
        time_duration runtime =
            *std::min_element(thisrun.begin(), thisrun.end());
        std::cout << n << " " << runtime.total_microseconds() << std::endl;
    }
    
}
