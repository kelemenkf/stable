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
    std::vector<double> correctedQuantiles;


public:
    QuantileEstimator(std::vector<double> sampleInput);

    ~QuantileEstimator();

    StableDistribution getParameters() const;
    
protected:
    void sortSample();
    
    void calculateQVector();

    std::vector<CartesianPoint> findAdjacentQuantiles(double quantile);

    double calculateCorrectedQuantile(const double& quantile);

    void initializeMemberQuantiles();

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

    std::vector<double> getSampleQs();
};

#endif