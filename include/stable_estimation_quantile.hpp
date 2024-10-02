#ifndef STABLE_ESTIMATION_QUANTILE_HPP
#define STABLE_ESTIMATION_QUANTILE_HPP

#include "stable_estimation.hpp"
#include "quantile_estimator_lookup_table.hpp"


class TableEntry 
{
public:
    double alpha, beta, vAlpha, vBeta;


public: 
    TableEntry(double alphaInput, double betaInput, double vAlphaInput, double vBetaInput) :
    alpha(alphaInput), beta(betaInput), vAlpha(vAlphaInput), vBeta(vBetaInput) {};


    double getAlpha()
    {
        return alpha; 
    }  

    double getBeta()
    {
        return beta;
    }

    double getVAlpha()
    {
        return vAlpha;
    }

    double getVBeta()
    {
        return vBeta;
    }

    void setVAlpha(double input)
    {
        vAlpha = input;
    }

    void setVBeta(double input)
    {
        vBeta = input;
    }
};


class QuantileEstimator: public Estimator
{
private:
    StableDistribution estimatedDistribution;
    double vAlphaSample;
    double vBetaSample;
    double vGammaSample;
    double VDeltaSample;
    std::vector<double> sampleQs;
    std::vector<double> correctedQuantiles;
    std::vector<TableEntry> lookupTable;


public:
    QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQs = {},
    std::vector<double> correctedQuantilesInput = {0, 0, 0, 0, 0});

    ~QuantileEstimator();

    void getParameters();
    
protected:
    std::pair<double, double> estimateAlpha();

    std::pair<double, double> estimateBeta();

    double estimateGamma();

    double estimateDelta();

    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> findAlphaPoints();

    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> findBetaPoints(
        std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints, bool estimateBeta = false);

    void populateTableAlphaBeta();

    void fillVector(std::vector<double>& parameters, double& mesh, double& paramMin, double& paramMax);

    void readLookupTableFromFile(const std::string& vFunction);
    
    void sortSample();
    
    void calculateQVector();

    std::vector<CartesianPoint> findAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector);

    double calculateCorrectedQuantile(const double& quantile);

    void initializeMemberQuantiles();

    void calculateVAlpha();

    void calculateVBeta();

    void calculateVGamma();

    void calculateVDelta();

    void setDistributionParameters();

    double getVAlpha();

    double getVBeta();

    std::vector<double> getSample();

    std::vector<double> getSampleQs();

    std::vector<double> getCorrectedQuantiles();
};

#endif