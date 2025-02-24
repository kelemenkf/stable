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
    double vGammaSample;
    double vDeltaSample;
    std::vector<double> sampleQs;
    std::vector<double> correctedQuantiles;


public:
    QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQs = {},
    std::vector<double> correctedQuantilesInput = {0, 0, 0, 0, 0});

    ~QuantileEstimator();

    void getParameters();

    double getVAlpha();

    double getVBeta();

    double getVGamma();

    double getVDelta();

    std::vector<double> getCorrectedQuantiles();
    
protected:
    std::vector<double> fillVector(std::vector<double>& parameters, double& mesh, double& paramMin, double& paramMax);

    double searchAlpha();

    double searchBeta();

    double calculateV(const std::string& param);
    
    void sortSample();
    
    void calculateQVector();

    std::vector<CartesianPoint> findAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector);

    double calculateCorrectedQuantile(const double& quantile);

    void initializeMemberQuantiles();

    void calculateVAlpha();

    void calculateVBeta();

    void calculateVGamma();

    void calculateVDelta();

    std::vector<double> getSample();

    std::vector<double> getSampleQs();
};

#endif