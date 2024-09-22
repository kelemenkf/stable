#include "stable_estimation_quantile.hpp"

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput) : Estimator(sampleInput) {};


QuantileEstimator::~QuantileEstimator()
{

}


QuantileEstimatorLookupTable QuantileEstimator::fillTable()
{
    QuantileEstimatorLookupTable lookupTables;
    lookupTables.calculateLookupTables();
    
    return lookupTables;
}