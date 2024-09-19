#ifndef STABLE_H
#define STABLE_H

#include <random>
#include <stdexcept>
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

    unsigned short getParametrization() const;

    void setParametrization(const unsigned short& param);

private:
    static double validateAlpha(double alpha);

    static double validateBeta(double beta);

    static unsigned short validateParametrizationIndex(unsigned short index);

    int getAlpha();
};

#endif