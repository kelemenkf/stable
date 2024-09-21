#include "quantile_estimator_lookup_table.hpp"
#include <fstream>

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
    Simulator simulator;
}


double QuantileEstimatorLookupTable::validateMesh(double mesh)
{
    if (mesh < 0 || mesh > 0.5)
    {
        throw std::invalid_argument("Mesh has range (0; 0.5]");
    }

    return mesh;
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