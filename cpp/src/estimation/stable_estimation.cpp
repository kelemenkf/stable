#include "stable_estimation.hpp"


Estimator::Estimator(std::vector<double> sampleInput) : sample(Estimator::validateSample(sampleInput)) {};


Estimator::~Estimator() 
{
    
};


std::vector<double> Estimator::validateSample(std::vector<double> sample)
{
    if (sample.empty())
    {
        throw std::invalid_argument("Sample contains no data");
    }

    return sample;
}