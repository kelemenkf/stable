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
    std::map<std::string, std::map<std::tuple<double, double>, double>> lookupTable;


public:
    QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQs = {},
    std::vector<double> correctedQuantilesInput = {0, 0, 0, 0, 0});

    ~QuantileEstimator();

    StableDistribution getParameters() const;
    
protected:
    std::vector<double> calculateAlpha();

    std::map<std::tuple<double, double>, double> getFirstColumnVAlpha();

    std::map<std::tuple<double, double>, double> getFirstColumnVBeta();

    std::vector<CartesianPoint> findAdjacentAlphas(double vValue, std::map<std::tuple<double, double>, double> vAlpha);

    void readLookupTableFromFile(const std::string& vFunction);
    
    void sortSample();
    
    void calculateQVector();

    std::vector<CartesianPoint> findAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector);

    double calculateCorrectedQuantile(const double& quantile);

    void initializeMemberQuantiles();

    void calculateVAlpha();

    void calculateVBeta();

    double calculateVGamma();

    double calculateVDelta();

    void setDistributionParameters();

    double getVAlpha();

    double getVBeta();

    std::vector<double> getSample();

    std::vector<double> getSampleQs();

    std::vector<double> getCorrectedQuantiles();
};

#endif