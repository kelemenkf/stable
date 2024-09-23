#ifndef QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP
#define QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP

#include "stable_simulation.hpp"
#include "utils.hpp"
#include <string>
#include <map>
#include <tuple>

class QuantileEstimatorLookupTable
{
private:
    std::map<std::string, std::map<std::tuple<double, double>, double>> lookupTables;
    double mesh;
    double alphaMin;
    double alphaMax;
    double betaMin;
    double betaMax;

    int sampleCounterForTest;


public:
    QuantileEstimatorLookupTable(double meshInput = 0.1, double alphaMinInput = 0.1, double alphaMaxInput = 2.0, 
    double betaMinInput = 0, double betaMaxInput = 1.0);

    std::map<std::tuple<double, double>, double> operator[] (const std::string& tableName);
    
    void writeLookupTablesToFile(const std::string& vFunction);

    void calculateLookupTables();

    std::map<std::string, double> calculateVFunctionValuesForAlphaBetaPair(const double& alpha, const double& beta);

private:
    static double validateMesh(double mesh);

    static double validateAlphaMin(double alphaMin);

    static double validateAlphaMax(double alphaMax);

    static double validateBetaMin(double betaMin);

    static double validateBetaMax(double betaMax);

protected:
    void fillAlphaVector(std::vector<double>& alphaValues);

    void fillBetaVector(std::vector<double>& betaValues);

    std::vector<double> calculateMeanOfSamples(std::vector<double>& sumOfSamples);

    std::vector<double> calculateSumOfSamples(const unsigned int& numberOfSamples);

    size_t getTableSize() const;

    double getMesh() const;

    double getAlphaMin() const;

    double getAlphaMax() const;

    double getBetaMin() const;

    double getBetaMax() const;

    int getSampleCounterForTest() const;
};

#endif