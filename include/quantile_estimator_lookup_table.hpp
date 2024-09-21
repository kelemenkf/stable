#ifndef QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP
#define QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP

#include "stable_simulation.hpp"
#include "utils.hpp"
#include <string>
#include <map>

class QuantileEstimatorLookupTable
{
private:
    std::map<std::string, std::vector<double>> table;
    double mesh;
    double alphaMin;
    double alphaMax;
    double betaMin;
    double betaMax;


public:
    QuantileEstimatorLookupTable(double meshInput = 0.1, double alphaMinInput = 0.1, double alphaMaxInput = 2.0, 
    double betaMinInput = 0, double betaMaxInput = 1.0);

    void calculateLookupTable();

    void writeLookupTableToFile();


private:
    static double validateMesh(double mesh);

    static double validateAlphaMin(double alphaMin);

    static double validateAlphaMax(double alphaMax);

    static double validateBetaMin(double betaMin);

    static double validateBetaMax(double betaMax);

    void fillAlphas(const std::vector<double>& alphaValues);

    void fillBetas(const std::vector<double>& betaValues);

    std::map<std::string, double> calculateV(const double& alpha, const double& beta);



protected:
    size_t getTableSize() const;

    double getMesh() const;

    double getAlphaMin() const;

    double getAlphaMax() const;

    double getBetaMin() const;

    double getBetaMax() const;
};

#endif