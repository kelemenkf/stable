#include "stable_estimation_quantile.hpp"

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput) : Estimator(sampleInput) {};


QuantileEstimator::~QuantileEstimator()
{

}


void QuantileEstimator::sortSample()
{
    std::sort(sample.begin(), sample.end(), [](double a, double b){ return a < b; });
}