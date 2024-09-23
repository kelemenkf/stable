#include "quantile_estimator_lookup_table.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

QuantileEstimatorLookupTable::QuantileEstimatorLookupTable(double meshInput, double alphaMinInput,
double alphaMaxInput, double betaMinInput, double betaMaxInput)
: mesh(QuantileEstimatorLookupTable::validateMesh(meshInput)), alphaMin(QuantileEstimatorLookupTable::validateAlphaMin(alphaMinInput)),
alphaMax(QuantileEstimatorLookupTable::validateAlphaMax(alphaMaxInput)), betaMin(QuantileEstimatorLookupTable::validateBetaMin(betaMinInput)), 
betaMax(QuantileEstimatorLookupTable::validateBetaMax(betaMaxInput)) {
    std::map<std::tuple<double, double>, double> vAlphaValues;
    std::map<std::tuple<double, double>, double> vBetaValues; 
    std::map<std::tuple<double, double>, double> vGammaValues; 
    std::map<std::tuple<double, double>, double> vDeltaValues; 

    lookupTables = {
        {"vAlpha", vAlphaValues },
        {"vBeta", vBetaValues },
        {"vGamma",  vGammaValues },
        {"vDelta", vDeltaValues }
    };   
};


std::map<std::tuple<double, double>, double> QuantileEstimatorLookupTable::operator[] (const std::string& tableName)
{
    return lookupTables[tableName];
}


void QuantileEstimatorLookupTable::writeLookupTablesToFile(const std::string& vFunction)
{
    std::vector<double> alphaValues;
    std::vector<double> betaValues;
    fillAlphaVector(alphaValues);
    fillBetaVector(betaValues);

    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";
    std::ofstream lookupTableFile(filePath, std::ios::trunc);

    if (lookupTableFile.is_open())
    {
        lookupTableFile << " ,";
        for(double beta: betaValues)
        {
            lookupTableFile << beta << ",";
        }
        lookupTableFile << '\n';

        for (auto alpha: alphaValues)
        {   
            lookupTableFile << alpha << ',';
            for (double beta: betaValues)
            {
                lookupTableFile << lookupTables[vFunction][std::tuple<double, double>(alpha, beta)] << ',';
            }
            lookupTableFile << '\n';
        }

        lookupTableFile.close();
    }
    else 
    {
        std::cerr << "Error opening file" << std::endl;
    }
}


void QuantileEstimatorLookupTable::calculateLookupTables(const unsigned int& numberOfSamples)
{
    std::vector<double> alphaValues;
    std::vector<double> betaValues;
    fillAlphaVector(alphaValues);
    fillBetaVector(betaValues);

    for (auto alpha = alphaValues.cbegin(); alpha != alphaValues.cend(); 
    ++alpha)
    {
        for (auto beta = betaValues.cbegin(); beta != betaValues.cend();
        ++beta)
        {
            std::tuple<double, double> index(*alpha, *beta);
            std::map<std::string, double> vValues = calculateVFunctionValuesForAlphaBetaPair(*alpha, *beta, numberOfSamples);
            lookupTables["vAlpha"][index] = vValues["alpha"];
            lookupTables["vBeta"][index] = vValues["beta"];
            lookupTables["vGamma"][index] = vValues["gamma"];
            lookupTables["vDelta"][index] = vValues["delta"];
        }
    }
} 


void QuantileEstimatorLookupTable::fillAlphaVector(std::vector<double>& alphaValues)
{
    alphaValues.resize(static_cast<size_t>((getAlphaMax() - getAlphaMin() + getMesh()) / getMesh()));

    double currentAlpha = this->getAlphaMin();

    auto alphaFill = [this, currentAlpha]() mutable -> double {
        double alpha = currentAlpha;
        
        if (currentAlpha <= this->getAlphaMax())
        {
            currentAlpha += getMesh();
        } 

        return alpha;
    };

    std::generate(alphaValues.begin(), alphaValues.end(), alphaFill);
}


void QuantileEstimatorLookupTable::fillBetaVector(std::vector<double>& betaValues)
{
    betaValues.resize(static_cast<size_t>((getBetaMax() - getBetaMin() + getMesh()) / getMesh()));

    double currentBeta = this->getBetaMin();

    auto betaFill = [this, currentBeta]() mutable -> double {
        double beta = currentBeta;

        if (currentBeta <= this->getBetaMax())
        {
            currentBeta+= getMesh();
        } 

        return beta;
    };

    std::generate(betaValues.begin(), betaValues.end(), betaFill);
}


std::map<std::string, double> QuantileEstimatorLookupTable::calculateVFunctionValuesForAlphaBetaPair(
    const double& alpha, const double& beta, const unsigned int& numberOfSamples)
{
    std::map<std::string, double> V = {
        {"alpha", 0},
        {"beta", 0},
        {"gamma", 0},
        {"delta", 0}
    };

    Simulator simulator(alpha, beta);
    std::vector<double> sumOfSamples = calculateSumOfSamples(numberOfSamples, simulator);
    std::vector<double> meanOfSamples = calculateMeanOfSamples(sumOfSamples, numberOfSamples);

    double vAlpha = (getQuantile(meanOfSamples, 0.95) - getQuantile(meanOfSamples, 0.05)) / 
    (getQuantile(meanOfSamples, 0.75) - getQuantile(meanOfSamples, 0.25));
    double vBeta = (getQuantile(meanOfSamples, 0.05) + getQuantile(meanOfSamples, 0.95) - 2*getQuantile(meanOfSamples, 0.50)) / 
    (getQuantile(meanOfSamples, 0.95) - getQuantile(meanOfSamples, 0.05));
    double vGamma = (getQuantile(meanOfSamples, 0.75) - getQuantile(meanOfSamples, 0.25));
    double vDelta = -getQuantile(meanOfSamples, -0.5);

    V["alpha"] = vAlpha;
    V["beta"] = vBeta;
    V["gamma"] = vGamma;
    V["delta"] = vDelta;

    return V;
}


std::vector<double> QuantileEstimatorLookupTable::calculateMeanOfSamples(std::vector<double>& sumOfSamples, const unsigned int& numberOfSamples)
{
    std::vector<double> meanOfSamples(10000);

    std::transform(sumOfSamples.begin(), sumOfSamples.end(), meanOfSamples.begin(), 
    [sumOfSamples, numberOfSamples](double value) { return value / numberOfSamples; });

    return meanOfSamples;
}


std::vector<double> QuantileEstimatorLookupTable::calculateSumOfSamples(const unsigned int& numberOfSamples, Simulator& simulator)
{
    std::vector<double> sumOfSamples(10000);

    int counter = 0;

    auto addVectors = [](double a, double b){  return a + b; };

    for (unsigned int i = 0; i < numberOfSamples; i++)
    {
        std::vector<double> sample = simulator.simulateNonSymmetricZVector(10000);  
        std::sort(sample.begin(), sample.end(), [](double a, double b){ return a < b; });
        std::transform(sumOfSamples.begin(), sumOfSamples.end(), sample.begin(), sumOfSamples.begin(), addVectors);
        counter++;
    }

    sampleCounterForTest = counter;

    return sumOfSamples;
}


double QuantileEstimatorLookupTable::getMesh() const
{
    return mesh;
}


double QuantileEstimatorLookupTable::getAlphaMin() const
{
    return alphaMin;
}


double QuantileEstimatorLookupTable::getAlphaMax() const
{
    return alphaMax;
}


double QuantileEstimatorLookupTable::getBetaMin() const
{
    return betaMin;
}


double QuantileEstimatorLookupTable::getBetaMax() const
{
    return betaMax;
}


size_t QuantileEstimatorLookupTable::getTableSize() const 
{
    return lookupTables.size();
}


int QuantileEstimatorLookupTable::getSampleCounterForTest() const
{
    return sampleCounterForTest;
}


double QuantileEstimatorLookupTable::validateMesh(double mesh)
{
    if (mesh < 0 || mesh > 0.5)
    {
        throw std::invalid_argument("Mesh shoud be in the range [0; 0.5]");
    }

    return mesh;
}


double QuantileEstimatorLookupTable::validateAlphaMin(double alphaMin)
{
    if (alphaMin < 0.1)
    {
        throw std::invalid_argument("Minimum alpha is 0.1");
    }

    return alphaMin;
}


double QuantileEstimatorLookupTable::validateAlphaMax(double alphaMax)
{
    if (alphaMax > 2.0)
    {
        throw std::invalid_argument("Maximum alpha is 2");
    }

    return alphaMax;
}



double QuantileEstimatorLookupTable::validateBetaMin(double betaMin)
{
    if (betaMin < 0)
    {
        throw std::invalid_argument("Minimum beta is 0");
    }

    return betaMin;
}



double QuantileEstimatorLookupTable::validateBetaMax(double betaMax)
{
    if (betaMax > 1.0)
    {
        throw std::invalid_argument("Maximum beta is 1");
    }

    return betaMax;
}