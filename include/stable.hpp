#ifndef STABLE_H
#define STABLE_H

#include <random>
#include <vector>

class StableDistribution
{
private:
    double alpha; 
    double beta;
    double gamma; 
    double delta; 
    int parametrization_index;
    std::mt19937 generator; 
    std::uniform_real_distribution<double> uniform_distribution;

public: 
    StableDistribution();
    
    StableDistribution(double alphaInput, double betaInput, double gammaInput,
    double deltaInput);

    ~StableDistribution();

    void getUniformBounds() const;

    int getParametrization() const;

    void setParametrization(const int& param);

    double generateStableX();

    double generateNonSymmetricZ();

    double generateSymmetricZ();

    double generateUniformNumber();

    double generateExponentialNumber();

    std::vector<double> generateStableXVector(int n = 10000);

    std::vector<double> generateNonSymmetricZVector(int n = 10000);

    std::vector<double> generateSymmetricZVector(int n = 10000);

    std::vector<double> generateExponentialVector(int n = 10000);

    std::vector<double> generateUniformVector(int n = 10000);
};

#endif