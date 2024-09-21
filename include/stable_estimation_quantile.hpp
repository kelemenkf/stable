#ifndef STABLE_ESTIMATION_QUANTILE_HPP
#define STABLE_ESTIMATION_QUANTILE_HPP

#include "stable_estimation.hpp"


class QuantileEstimator: private Estimator
{
private:
    StableDistribution estimatedDistribution;


public:
    QuantileEstimator(std::vector<double> sample);

    ~QuantileEstimator();

    //Sample sorted. Five quantiles determined with correction.
};

#endif