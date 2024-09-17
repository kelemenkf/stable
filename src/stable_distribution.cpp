#include <iostream>
#include <cmath>
#include <algorithm>
#include "stable_distribution.hpp"

StableDistribution::StableDistribution() {};


StableDistribution::StableDistribution(double alphaInput, double betaInput, double gammaInput,
double deltaInput): alpha(alphaInput), beta(betaInput), gamma(gammaInput), 
delta(deltaInput), parametrizationIndex(0) {};


StableDistribution::~StableDistribution()
{

}


static void setRandomEngine() 
{

}


void StableDistribution::getUniformBounds() const
{
    auto params = uniform_distribution.param();
    std::cout << params.a() << std::endl;
    std::cout << params.b() << std::endl;
}


int StableDistribution::getParametrization() const
{
    return parametrizationIndex;
}


void StableDistribution::setParametrization(const int& param)
{
    parametrizationIndex = param;
}