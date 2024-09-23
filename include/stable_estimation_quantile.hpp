#ifndef STABLE_ESTIMATION_QUANTILE_HPP
#define STABLE_ESTIMATION_QUANTILE_HPP

#include "stable_estimation.hpp"
#include "quantile_estimator_lookup_table.hpp"


class QuantileEstimator: public Estimator
{
private:
    StableDistribution estimatedDistribution;


public:
    QuantileEstimator(std::vector<double> sample);

    ~QuantileEstimator();

    
private:
    void sortSample();

    double calculateVAlpha();

    double calculateVBeta();

    double calculateVGamma();

    double calculateVDelta();

    void readLookupTableFromFile();

    std::tuple<double, double> findParameterRangeFromLookupTable(const std::map<std::tuple<double, double>, double>& lookupTable);

    double interpolateParameter(std::tuple<double, double> paramRange);

    void setDistributionParameters();
};

#endif