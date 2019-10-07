/* main.cpp */

#include <iostream>
#include <string>
#include "part_a.h"
#include "part_b.h"
#include "read_data.h"
   
int main(int argc, char *argv[])
{
    // read the data
    std::string fileName = "../data/asset_returns.csv";
    Matrix<double> returnMatrix = readData<double>(fileName);
    std::size_t nAssets = returnMatrix.get_col();
    std::size_t nObs = returnMatrix.get_row();

    // set up the quadratic program
    std::vector<double> onesVec(nAssets, 1);
    Matrix<double> ones(onesVec, nAssets, 1);
    Matrix<double> b(nAssets + 2, 1);
    b.at(nAssets + 1, 0) = -1;

    // procedure settings
    int window = 100;
    int count = 0;
    int step = 12;
    int beg = 0;
    int end = window;
    std::vector<double> targetList = {0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05,
                                      0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1};

    // initialise two vectors to store results
    std::vector<double> portMeans;
    std::vector<double> portVars;

    while(end < nObs){
        // get in sample data, covariance matrix and mean vector
        Matrix<double> insampleReturns = returnMatrix.slice(beg, end, 0, nAssets);
        Matrix<double> insampleCov = getCovMatrix(insampleReturns, window, 0);
        Matrix<double> insampleMean = getMean(insampleReturns).t();

        // get out of sample data, covariance matrix and mean vector
        Matrix<double> oosReturns = returnMatrix.slice(end, end + step, 0, nAssets);
        Matrix<double> oosCov = getCovMatrix(oosReturns, step, 0);
        Matrix<double> oosMean = getMean(oosReturns).t();

        Matrix<double> Q = getQMatrix(insampleCov, -insampleMean, -ones);

        for(const auto& target : targetList) {
            b.at(nAssets, 0) = -target;
            Matrix<double> x = random<double>(nAssets + 2, 1);
            conjugateGradient(x, Q, b, 1e-6, 1e6);
            Matrix<double> portWeights = x.slice(0, nAssets, 0, 1);
            portMeans.push_back(dotproduct(oosMean, portWeights));
            portVars.push_back(dotproduct(portWeights, mmult(oosCov, portWeights)));
        }
        beg += step;
        end += step;
        ++count;
    }
    
    Matrix<double> resultMeans(portMeans, count, targetList.size());
    Matrix<double> resultVars(portVars, count, targetList.size());

    std::cout << "OOS portfolio means:\n";
    std::cout << resultMeans << std::endl;
    std::cout << "OOS portfolio variances:\n";
    std::cout << resultVars << std::endl;

    // export the resulting matrices to csv files
    to_csv(resultMeans, "resultMeans.csv");
    to_csv(resultVars, "resultVars.csv");

    return 0;
}
