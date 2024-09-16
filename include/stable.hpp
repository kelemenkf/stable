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

    double generateUniformNumber();

    double generateExponentialNumber();

    double generateSymmetricStableNumber();

    double generateNonSymmetricStableNumber();

    std::vector<double> generateUniformVector(int n);

    std::vector<double> generateExponentialVector(int n);
};

#endif