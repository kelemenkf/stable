#include "stable_estimation_quantile.hpp"
#include <iostream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQsInput, std::vector<double>
correctedQuantilesInput) 
: Estimator(sampleInput), sampleQs(sampleQsInput), correctedQuantiles(correctedQuantilesInput) {
    sortSample();
    calculateQVector();
    initializeMemberQuantiles();
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


void QuantileEstimator::initializeMemberQuantiles()
{
    std::vector<double> quantilesNeeded {0.05, 0.25, 0.5, 0.75, 0.95};
    for (size_t index = 0; index < quantilesNeeded.size(); ++index)
    {
        correctedQuantiles[index] = calculateCorrectedQuantile(quantilesNeeded[index]);
    }
}


double QuantileEstimator::calculateCorrectedQuantile(const double& quantile)
{
    std::vector<CartesianPoint> adjacentQuantiles = findAdjacentQuantiles(quantile);

    return linearInterpolation(adjacentQuantiles[0], adjacentQuantiles[1], quantile);
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


void QuantileEstimator::calculateVAlpha()
{
    vAlphaSample = (correctedQuantiles[4] - correctedQuantiles[0]) / 
    (correctedQuantiles[3] - correctedQuantiles[1]);
}


void QuantileEstimator::calculateVBeta()
{
    vBetaSample = (correctedQuantiles[0] + correctedQuantiles[4] - 2*correctedQuantiles[2]) / 
    (correctedQuantiles[4] - correctedQuantiles[0]);
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
