#ifndef STABLE_ESTIMATION_HPP
#define STABLE_ESTIMATION_HPP

#include "stable_distribution.hpp"

class Estimator
{
protected: 
    std::vector<double> sample;


public:
    Estimator(std::vector<double> sampleInput);

    ~Estimator();

    StableDistribution getParameters() const;


private: 
    static std::vector<double> validateSample(std::vector<double> sample);
};

#endif