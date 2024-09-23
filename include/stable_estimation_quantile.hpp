#ifndef STABLE_ESTIMATION_QUANTILE_HPP
#define STABLE_ESTIMATION_QUANTILE_HPP

#include "stable_estimation.hpp"
#include "quantile_estimator_lookup_table.hpp"


class QuantileEstimator: public Estimator
{
private:
    StableDistribution estimatedDistribution;
    double vAlphaSample;
    double vBetaSample;
    std::vector<double> sampleQs;


public:
    QuantileEstimator(std::vector<double> sampleInput);

    ~QuantileEstimator();

    StableDistribution getParameters() const;
    
protected:
    void sortSample();
    
    void calculateQVector();

    double correctQuantile(CartesianPoint percentiles, CartesianPoint sampleValues);

    void calculateVAlpha();

    void calculateVBeta();

    double calculateVGamma();

    double calculateVDelta();

    void readLookupTableFromFile();

    std::tuple<double, double> findParameterRangeFromLookupTable(const std::map<std::tuple<double, double>, double>& lookupTable);

    double interpolateParameter(std::tuple<double, double> paramRange);

    void setDistributionParameters();

    double getVAlpha();

    double getVBeta();

    std::vector<double> getSample();
};

#endif