#include "quantile_estimator_lookup_table.hpp"
#include <fstream>

QuantileEstimatorLookupTable::QuantileEstimatorLookupTable(double meshInput, double alphaMinInput,
double alphaMaxInput, double betaMinInput, double betaMaxInput)
: mesh(meshInput), alphaMin(alphaMinInput), alphaMax(alphaMaxInput), betaMin(betaMinInput), 
betaMax(betaMaxInput) {};


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
