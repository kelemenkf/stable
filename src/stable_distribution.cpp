#include <iostream>
#include <cmath>
#include <algorithm>
#include "stable_distribution.hpp"


std::mt19937 StableDistribution::generator(std::random_device{}());
std::uniform_real_distribution<double> StableDistribution::uniform_distribution(0.0, 1.0);


StableDistribution::StableDistribution(double alphaInput, double betaInput, double gammaInput,
double deltaInput, unsigned short parametrizationIndexInput) : alpha(StableDistribution::validateAlpha(alphaInput)), 
beta(StableDistribution::validateBeta(betaInput)), gamma(gammaInput), delta(deltaInput), 
parametrizationIndex(StableDistribution::validateParametrizationIndex(parametrizationIndexInput)) 
{};


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


unsigned short StableDistribution::getParametrization() const
{
    return parametrizationIndex;
}


void StableDistribution::setParametrization(const unsigned short& param)
{
    parametrizationIndex = StableDistribution::validateParametrizationIndex(param);;
}


int StableDistribution::getAlpha()
{
    return alpha;
}


double StableDistribution::validateAlpha(double alpha)
{    
    if (alpha > 2 || alpha <= 0)
    {
        throw std::invalid_argument("Alpha out of range (0;2]");
    }

    return alpha;
}


double StableDistribution::validateBeta(double beta)
{
    if (beta < -1 || beta > 1)
    {
        throw std::invalid_argument("Beta out of range [-1;1]");
    }

    return beta;
}


unsigned short StableDistribution::validateParametrizationIndex(unsigned short index)
{
    if (index != 0 && index != 1)
    {
        throw std::invalid_argument("Not a valid parametrization");
    }

    return index;
}