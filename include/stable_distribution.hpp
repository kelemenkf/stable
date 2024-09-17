#ifndef STABLE_H
#define STABLE_H

#include <random>
#include <vector>

class StableDistribution
{
protected:
    static std::mt19937 generator; 
    static std::uniform_real_distribution<double> uniform_distribution;

    double alpha; 
    double beta;
    double gamma; 
    double delta; 
    unsigned short parametrizationIndex;

public: 
    static void setRandomEngine();
    
    StableDistribution(double alphaInput = 2, double betaInput = 0, double gammaInput = 1,
    double deltaInput = 0, unsigned short parameterizationIndexInput = 0);

    ~StableDistribution();

    void getUniformBounds() const;

    int getParametrization() const;

    void setParametrization(const int& param);
};


std::mt19937 StableDistribution::generator(std::random_device{}());
std::uniform_real_distribution<double> StableDistribution::uniform_distribution(0.0, 1.0);

#endif