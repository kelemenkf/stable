#ifndef STABLE_SIMUALTI0N_HPP
#define STABLE_SIMULATION_HPP

#include "stable_distribution.hpp"

class Simulator: private StableDistribution
{
private: 
    std::vector<double> numbers;

public:
    Simulator();

    Simulator(double alphaInput, double betaInput, double gammaInput, double deltaInput, 
    unsigned short parametrizationIndexInput);

    ~Simulator();

    std::vector<double> generateStableXVector(unsigned int n);

    std::vector<double> generateNonSymmetricZVector(unsigned int n);

    std::vector<double> generateSymmetricZVector(unsigned int n);

    std::vector<double> generateExponentialVector(unsigned int n);

    std::vector<double> generateUniformVector(unsigned int n);

private:
    double generateStableX();

    double generateNonSymmetricZ();

    double generateSymmetricZ();

    double generateUniformNumber();

    double generateExponentialNumber();
};


#endif