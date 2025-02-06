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

    std::vector<double> simulateStableXVector(unsigned int n = 1);

    std::vector<double> simulateNonSymmetricZVector(unsigned int n = 1);

    std::vector<double> simulateSymmetricZVector(unsigned int n = 1);

    std::vector<double> simulateExponentialVector(unsigned int n = 1);

    std::vector<double> simulateUniformVector(unsigned int n = 1);

    unsigned short getParametrizationIndex() const;

private:
    double simulateStableX();

    double simulateNonSymmetricZ();

    double simulateSymmetricZ();

    double simulateUniformNumber();

    double simulateExponentialNumber();

    static unsigned int validateN(const unsigned int& n);
};


#endif