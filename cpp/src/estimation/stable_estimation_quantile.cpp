#include "stable_estimation_quantile.hpp"
#include <iostream>


QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQsInput, std::vector<double>
correctedQuantilesInput) 
: Estimator(sampleInput), sampleQs(sampleQsInput), correctedQuantiles(correctedQuantilesInput) {
    sortSample();
    calculateQVector();
    initializeMemberQuantiles();
    calculateVAlpha();
    calculateVBeta();
    calculateVGamma();
    calculateVDelta();
    searchAlpha();
};


QuantileEstimator::~QuantileEstimator()
{

}


double QuantileEstimator::searchAlpha()
{     
    std::vector<double> alphas; 
    double alphaMin = 0.5; 
    double alphaMax = 2.0; 
    double mesh = 0.1;

    alphas = fillVector(alphas, mesh, alphaMin, alphaMax);

    double vAlpha = getVAlpha();
}


std::vector<double> QuantileEstimator::fillVector(std::vector<double>& parameters, double& mesh, double& paramMin, double& paramMax)
{
    parameters.resize(static_cast<size_t>((paramMax - paramMin + mesh) / mesh));

    double currentParam = paramMin;

    auto vectorFill = [paramMax, mesh, &currentParam]() mutable -> double {
        double param = currentParam;
        
        if (currentParam <= paramMax)
        {
            currentParam += mesh;
        } 

        return param;
    };

    std::generate(parameters.begin(), parameters.end(), vectorFill);

    return parameters;
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
    std::vector<CartesianPoint> adjacentQuantiles = findAdjacentQuantiles(quantile, sampleQs, sample);

    return linearInterpolation(adjacentQuantiles[0], adjacentQuantiles[1], quantile);
} 


std::vector<CartesianPoint> QuantileEstimator::findAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector)
{
    CartesianPoint before;
    CartesianPoint after;

    size_t counter = 0;
    auto findQuantile = [yVector, &quantile, &counter, &before, &after](double value) mutable {
        if (quantile == value)
        {
            before.setX(value);
            after.setX(value);  
            before.setY(yVector[counter]);
            after.setY(yVector[counter]);
        }
        ++counter;
    };

    for_each(sampleQs.begin(), sampleQs.end(), findQuantile);

    for (size_t index = 0; index < sampleQs.size(); index++)
    {
        if (sampleQs[index - 1] < quantile && sampleQs[index] > quantile)
        {
            before.setX(xVector[index - 1]);
            after.setX(xVector[index]);
            before.setY(yVector[index - 1]);
            after.setY(yVector[index]);
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


void QuantileEstimator::calculateVGamma()
{
    vGammaSample = (correctedQuantiles[3] - correctedQuantiles[1]);
}


void QuantileEstimator::calculateVDelta()
{
    vDeltaSample = -correctedQuantiles[2];
}


double QuantileEstimator::getVAlpha()
{
    return vAlphaSample;
}


double QuantileEstimator::getVBeta()
{
    return vBetaSample;
}


double QuantileEstimator::getVGamma()
{
    return vGammaSample;
}


double QuantileEstimator::getVDelta()
{
    return vDeltaSample;
}


std::vector<double> QuantileEstimator::getSample()
{
    return sample;
}


std::vector<double> QuantileEstimator::getSampleQs()
{
    return sampleQs;
}


std::vector<double> QuantileEstimator::getCorrectedQuantiles()
{
    return correctedQuantiles;
}