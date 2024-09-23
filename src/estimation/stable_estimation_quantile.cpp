#include "stable_estimation_quantile.hpp"
#include "utils.hpp"
#include <iostream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput) : Estimator(sampleInput) {
    sortSample();
};


QuantileEstimator::~QuantileEstimator()
{

}


void QuantileEstimator::calculateVAlpha()
{
    vAlphaSample = (getQuantile(sample, 0.95) - getQuantile(sample, 0.05)) / 
    (getQuantile(sample, 0.75) - getQuantile(sample, 0.25));
}


void QuantileEstimator::calculateVBeta()
{
    vBetaSample = (getQuantile(sample, 0.05) + getQuantile(sample, 0.95) - 2*getQuantile(sample, 0.50)) / 
    (getQuantile(sample, 0.95) - getQuantile(sample, 0.05));
}


void QuantileEstimator::sortSample()
{
    std::sort(sample.begin(), sample.end(), [](double a, double b){ return a < b; });
}


double QuantileEstimator::getVAlpha()
{
    return vAlphaSample;
}


double QuantileEstimator::getVBeta()
{
    return vBetaSample;
}
