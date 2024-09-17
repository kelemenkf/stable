#ifndef STABLE_H
#define STABLE_H

#include <random>
#include <vector>

class StableDistribution
{
private:
    static std::mt19937 generator; 
    static std::uniform_real_distribution<double> uniform_distribution;

    double alpha; 
    double beta;
    double gamma; 
    double delta; 
    int parametrizationIndex;

public: 
    static void setRandomEngine();

    StableDistribution();
    
    StableDistribution(double alphaInput, double betaInput, double gammaInput,
    double deltaInput);

    ~StableDistribution();

    void getUniformBounds() const;

    int getParametrization() const;

    void setParametrization(const int& param);

    std::vector<double> generateStableXVector(int n = 10000);

    std::vector<double> generateNonSymmetricZVector(int n = 10000);

    std::vector<double> generateSymmetricZVector(int n = 10000);

    std::vector<double> generateExponentialVector(int n = 10000);

    std::vector<double> generateUniformVector(int n = 10000);

private:
    double generateStableX();

    double generateNonSymmetricZ();

    double generateSymmetricZ();

    double generateUniformNumber();

    double generateExponentialNumber();
};


std::mt19937 StableDistribution::generator(std::random_device{}());
std::uniform_real_distribution<double> StableDistribution::uniform_distribution(0.0, 1.0);

#endif