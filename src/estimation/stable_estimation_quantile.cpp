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


std::vector<CartesianPoint> QuantileEstimator::findAdjacentQuantiles(double quantile)
{
    CartesianPoint before;
    CartesianPoint after;

    size_t counter = 0;
    auto findQuantile = [this, &quantile, &counter, &before, &after](double value) mutable {
        if (quantile == value)
        {
            before.setX(value);
            after.setX(value);
            before.setY(sample[counter]);
            after.setY(sample[counter]);
        }
        ++counter;
    };

    for_each(sampleQs.begin(), sampleQs.end(), findQuantile);

    for (size_t index = 0; index < sampleQs.size(); index++)
    {
        if (sampleQs[index - 1] < quantile && sampleQs[index] > quantile)
        {
            before.setX(sampleQs[index - 1]);
            after.setX(sampleQs[index]);
            before.setY(sample[index - 1]);
            after.setY(sample[index]);
        }
    }

    std::vector<CartesianPoint> adjacentPoints{before, after};
    return adjacentPoints;
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
