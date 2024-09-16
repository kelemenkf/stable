#include <iostream>
#include <numbers>
#include <cmath>
#include "stable.hpp"
using namespace std::chrono;

StableDistribution::StableDistribution(): generator(12345), uniform_distribution(0, 1) {};

StableDistribution::StableDistribution(double alphaInput, double betaInput, double gammaInput,
double deltaInput): alpha(alphaInput), beta(betaInput), gamma(gammaInput), 
delta(deltaInput), generator(std::random_device{}()), uniform_distribution(0, 1), parametrization_index(0) {};

StableDistribution::~StableDistribution()
{

}

void StableDistribution::getUniformBounds() const
{
    auto params = uniform_distribution.param();
    std::cout << params.a() << std::endl;
    std::cout << params.b() << std::endl;
}

double StableDistribution::generateUniformNumber()
{
    double number;
    number = std::numbers::pi * (uniform_distribution(generator) - 0.5);  

    return number;
}

double StableDistribution::generateExponentialNumber()
{
    double number;
    number = -std::log(uniform_distribution(generator));

    return number;
}

double StableDistribution::generateSymmetricZ()
{
    double number;
    double uniform = generateUniformNumber();

    if (alpha == 1)
    {
        number = tan(uniform);
    }
    else
    { 
        double exponential = generateExponentialNumber();
        number = (sin(alpha * uniform) / pow(cos(uniform), (1/alpha))) * pow(((cos((alpha - 1) * uniform))/ exponential), ((1-alpha)/alpha));
    }

    return number;
}


double StableDistribution::generateNonSymmetricZ()
{
    double number;
    double uniform = generateUniformNumber();
    double exponential = generateExponentialNumber();

    if (alpha == 1)
    {
        number = (2/std::numbers::pi) * (((std::numbers::pi/2) + (beta*uniform)) * tan(uniform) - beta * log(((std::numbers::pi/2 * exponential * cos(uniform)))/((std::numbers::pi/2) + beta*uniform)));
    }     
    else
    {
        double theta_0 = atan(beta*tan(std::numbers::pi*alpha/2)) / alpha;
        number = (sin(alpha * (theta_0 + uniform)) / pow(cos(alpha*theta_0) * cos(uniform), (1/alpha))) * pow(((cos(alpha*theta_0 + (alpha - 1) * uniform))/ exponential), ((1-alpha)/alpha));
    }

    return number;
}


double StableDistribution::generateSymmetricX()
{
    double number;
    double symmetricZ = generateNonSymmetricZ();

    if (parametrization_index == 0)
    {
        if (alpha == 1)
        {
            number = (gamma * symmetricZ) + delta;
        }
        else
        {
            number = gamma * (symmetricZ - beta * tan((std::numbers::pi * alpha )/ 2)) + delta;
        }
    }
    else if (parametrization_index == 1)
    {
        if (alpha == 1)
        {
            number = (gamma * symmetricZ) + (delta + beta * (2/std::numbers::pi) * gamma * log(gamma));
        }
        else
        {
            number = (gamma * symmetricZ) + delta;
        }
    }

    return number;
}


std::vector<double> StableDistribution::generateUniformVector(int n)
{
    std::vector<double> numbers(n);

    for (auto element = numbers.begin(); element != numbers.end();
    ++element)
    {
        *element = generateUniformNumber();
    }

    return numbers;
}


std::vector<double> StableDistribution::generateExponentialVector(int n)
{
    std::vector<double> numbers(n);
     
    for (auto element = numbers.begin(); element != numbers.end(); 
    ++element)
    {
        *element = generateExponentialNumber();
    }

    return numbers;
}