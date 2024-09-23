#include "stable_estimation_quantile.hpp"
#include <iostream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput) : Estimator(sampleInput) {
    sortSample();
};


QuantileEstimator::~QuantileEstimator()
{

}


void QuantileEstimator::sortSample()
{
    std::sort(sample.begin(), sample.end(), [](double a, double b){ return a < b; });
}


void QuantileEstimator::calculateQVector()
{
    sampleQs.resize(sample.size());

    int counter = 1;
    auto fillQVector = [this, &counter]() mutable { 
        return (2.0*(counter++) - 1) / (2 * this->sampleQs.size()); 
    };

    std::generate(sampleQs.begin(), sampleQs.end(), fillQVector);
}


double QuantileEstimator::correctQuantile(CartesianPoint percentiles, CartesianPoint sampleValuse)
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


double QuantileEstimator::getVAlpha()
{
    return vAlphaSample;
}


double QuantileEstimator::getVBeta()
{
    return vBetaSample;
}


std::vector<double> QuantileEstimator::getSample()
{
    return sample;
}


std::vector<double> QuantileEstimator::getSampleQs()
{
    return sampleQs;
}
