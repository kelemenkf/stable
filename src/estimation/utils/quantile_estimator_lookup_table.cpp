#include "quantile_estimator_lookup_table.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>

QuantileEstimatorLookupTable::QuantileEstimatorLookupTable(double meshInput, double alphaMinInput,
double alphaMaxInput, double betaMinInput, double betaMaxInput)
: mesh(QuantileEstimatorLookupTable::validateMesh(meshInput)), alphaMin(QuantileEstimatorLookupTable::validateAlphaMin(alphaMinInput)),
alphaMax(QuantileEstimatorLookupTable::validateAlphaMax(alphaMaxInput)), betaMin(QuantileEstimatorLookupTable::validateBetaMin(betaMinInput)), 
betaMax(QuantileEstimatorLookupTable::validateBetaMax(betaMaxInput)) {};


void QuantileEstimatorLookupTable::writeLookupTableToFile()
{
    std::ofstream lookupTableStream;

    lookupTableStream.open("lookup_table.csv", std::ios::out);

    if (lookupTableStream.is_open()) 
    {
    }
}


void QuantileEstimatorLookupTable::calculateLookupTable()
{
    std::vector<double> alphaValues;
    std::vector<double> betaValues;
    fillAlphas(alphaValues);
    fillBetas(betaValues);
} 


void QuantileEstimatorLookupTable::fillAlphas(const std::vector<double>& alphaValues)
{
    std::vector<double> alphaValues(static_cast<size_t>(getAlphaMax() / getMesh()));

    double currentAlpha = this->getAlphaMin();

    auto alphaFill = [this, currentAlpha]() mutable -> double {
        while(currentAlpha <= this->getAlphaMax())
        {
            return currentAlpha++;
        } 
    };

    std::transform(alphaValues.begin(), alphaValues.end(), alphaValues.begin(), alphaFill);
}


void QuantileEstimatorLookupTable::fillBetas(const std::vector<double>& betaValues)
{
    std::vector<double> alphaValues(static_cast<size_t>((getBetaMax() + getMesh()) / getMesh()));

    double currentBeta = this->getBetaMin();

    auto betaFill = [this, currentBeta]() mutable -> double {
        while(currentBeta <= this->getBetaMax())
        {
            return currentBeta++;
        } 
    };

    std::transform(betaValues.begin(), betaValues.end(), betaValues.begin(), betaFill);
}


std::map<std::string, double> QuantileEstimatorLookupTable::calculateV(const double& alpha, const double& beta)
{
    std::map<std::string, double> V = {
        {"alpha", 0},
        {"beta", 0}
    };

    //Needs 0 parametrization, which is the default.
    Simulator simulator(alpha, beta);
    std::vector<double> sample;
    sample = simulator.simulateSymmetricZVector(100000);

    std::sort(sample.begin(), sample.end(), [](double a, double b) {
        return a < b;
    });

    double vAlpha = (getQuantile(sample, 0.95) - getQuantile(sample, 0.05)) / 
    (getQuantile(sample, 0.75) - getQuantile(sample, 0.25));
    double vBeta = (getQuantile(sample, 0.05) + getQuantile(sample, 0.95) - 2*getQuantile(sample, 0.50)) / 
    (getQuantile(sample, 0.95) - getQuantile(sample, 0.05));
    V["alpha"] = vAlpha;
    V["beta"] = vBeta;

    return V;
}



double QuantileEstimatorLookupTable::getMesh() const
{
    return mesh;
}


double QuantileEstimatorLookupTable::getAlphaMin() const
{
    return alphaMin;
}


double QuantileEstimatorLookupTable::getAlphaMax() const
{
    return alphaMax;
}


double QuantileEstimatorLookupTable::getBetaMin() const
{
    return betaMin;
}


double QuantileEstimatorLookupTable::getBetaMax() const
{
    return betaMax;
}


size_t QuantileEstimatorLookupTable::getTableSize() const 
{
    return table.size();
}


double QuantileEstimatorLookupTable::validateAlphaMin(double alphaMin)
{
    if (alphaMin < 0.1)
    {
        throw std::invalid_argument("Minimum alpha is 0.1");
    }

    return alphaMin;
}


double QuantileEstimatorLookupTable::validateAlphaMax(double alphaMax)
{
    if (alphaMax > 2.0)
    {
        throw std::invalid_argument("Maximum alpha is 2");
    }

    return alphaMax;
}



double QuantileEstimatorLookupTable::validateBetaMin(double betaMin)
{
    if (betaMin < 0)
    {
        throw std::invalid_argument("Minimum beta is 0");
    }

    return betaMin;
}



double QuantileEstimatorLookupTable::validateBetaMax(double betaMax)
{
    if (betaMax > 1.0)
    {
        throw std::invalid_argument("Maximum beta is 1");
    }

    return betaMax;
}