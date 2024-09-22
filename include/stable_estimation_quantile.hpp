#ifndef STABLE_ESTIMATION_QUANTILE_HPP
#define STABLE_ESTIMATION_QUANTILE_HPP

#include "stable_estimation.hpp"
#include "quantile_estimator_lookup_table.hpp"


class QuantileEstimator: private Estimator
{
private:
    StableDistribution estimatedDistribution;
    

public:
    QuantileEstimator(std::vector<double> sample);

    ~QuantileEstimator();

    
private:
    QuantileEstimatorLookupTable fillTable();
};

#endif