#include <iostream>
#include <cmath>
#include <algorithm>
#include "stable_distribution.hpp"


std::mt19937 StableDistribution::generator(std::random_device{}());
std::uniform_real_distribution<double> StableDistribution::uniform_distribution(0.0, 1.0);


StableDistribution::StableDistribution(double alphaInput, double betaInput, double gammaInput,
double deltaInput, unsigned short parametrizationIndexInput) : alpha(alphaInput), beta(betaInput), gamma(gammaInput), 
delta(deltaInput), parametrizationIndex(parametrizationIndexInput) {};


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