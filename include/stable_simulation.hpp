#ifndef STABLE_SIMULATION_HPP
#define STABLE_SIMULATION_HPP

#include "stable_distribution.hpp"

class Simulator: private StableDistribution
{
private: 
    unsigned int n;
    std::vector<double> numbers;

public:
    Simulator(double alphaInput = 2, double betaInput = 0, double gammaInput = 1, double deltaInput = 0, 
    unsigned short parametrizationIndexInput = 0, unsigned int nInput = 1);

    ~Simulator();

    std::vector<double> generateStableXVector(unsigned int n = 1);

    std::vector<double> generateNonSymmetricZVector(unsigned int n = 1);

    std::vector<double> generateSymmetricZVector(unsigned int n = 1);

    std::vector<double> generateExponentialVector(unsigned int n = 1);

    std::vector<double> generateUniformVector(unsigned int n = 1);

private:
    double generateStableX();

    double generateNonSymmetricZ();

    double generateSymmetricZ();

    double generateUniformNumber();

    double generateExponentialNumber();

    static unsigned int validateN(const unsigned int& n);
};


#endif