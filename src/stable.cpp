#include <iostream>
#include <numbers>
#include <cmath>
#include <algorithm>
#include "stable.hpp"
using namespace std::chrono;

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


std::vector<double> StableDistribution::generateStableXVector(int n)
{
    std::vector<double> numbers(n);

    auto stableLambda = [this]() -> double { return this->generateStableX(); };
    std::generate(numbers.begin(), numbers.end(), stableLambda);

    return numbers;
}


std::vector<double> StableDistribution::generateSymmetricZVector(int n)
{
    std::vector<double> numbers(n);

    auto symmetricZLambda = [this]() -> double { return this->generateNonSymmetricZ(); };
    std::generate(numbers.begin(), numbers.end(), symmetricZLambda);

    return numbers;
}


std::vector<double> StableDistribution::generateNonSymmetricZVector(int n)
{
    std::vector<double> numbers(n);

    auto nonSymmetricZLambda = [this]() -> double { return this->generateNonSymmetricZ(); };
    std::generate(numbers.begin(), numbers.end(), nonSymmetricZLambda);

    return numbers;
}


std::vector<double> StableDistribution::generateExponentialVector(int n)
{
    std::vector<double> numbers(n);
     
    auto exponentialLambda = [this]() -> double { return this->generateExponentialNumber(); };
    std::generate(numbers.begin(), numbers.end(), exponentialLambda);

    return numbers;
}


std::vector<double> StableDistribution::generateUniformVector(int n)
{
    std::vector<double> numbers(n);

    auto uniformLambda = [this]() -> double { return this->generateUniformNumber(); };
    std::generate(numbers.begin(), numbers.end(), uniformLambda);

    return numbers;
}


double StableDistribution::generateStableX()
{
    /*

    Can be used to generate both 0 and 1 paramterized variables, even though Z is
    generated using the 1 parametrization. 
    
    */
    double number;
    double Z = generateNonSymmetricZ();

    if (parametrizationIndex == 0)
    {
        if (alpha == 1)
        {
            number = (gamma * Z) + delta;
        }
        else
        {
            number = gamma * (Z - beta * tan((std::numbers::pi * alpha )/ 2)) + delta;
        }
    }
    else if (parametrizationIndex == 1)
    {
        if (alpha == 1)
        {
            number = (gamma * Z) + (delta + beta * (2/std::numbers::pi) * gamma * log(gamma));
        }
        else
        {
            number = (gamma * Z) + delta;
        }
    }

    return number;
}


double StableDistribution::generateSymmetricZ()
{
    //Generates a symmetric Z with the 1 parametrization. 
    double number;
    double uniform = generateUniformNumber();

    if (alpha == 1)
    {
        number = tan(uniform);
    }
    else
    { 
        double exponential = generateExponentialNumber();
        number = (sin(alpha*uniform) / pow(cos(uniform), (1/alpha))) * pow((cos((alpha - 1) * uniform)) / exponential, (1-alpha) / alpha);
    }

    return number;
}


double StableDistribution::generateNonSymmetricZ()
{
    //Generates a nonsymmetric Z with the 1 parametrization. 
    double number;
    double uniform = generateUniformNumber();
    double exponential = generateExponentialNumber();

    if (alpha == 1)
    {
        number = (2/std::numbers::pi) * (((std::numbers::pi/2) + (beta*uniform)) * tan(uniform) - beta * log(((std::numbers::pi/2 * exponential*cos(uniform)))/((std::numbers::pi/2) + beta*uniform)));
    }     
    else
    {
        double theta_0 = atan(beta*tan(std::numbers::pi*alpha/2)) / alpha;
        number = (sin(alpha * (theta_0 + uniform)) / pow(cos(alpha*theta_0)*cos(uniform), (1/alpha))) * pow(((cos(alpha*theta_0 + (alpha - 1)*uniform))/ exponential), ((1-alpha)/alpha));
    }

    return number;
}


double StableDistribution::generateExponentialNumber()
{
    double number;

    number = -std::log(uniform_distribution(generator));

    return number;
}


double StableDistribution::generateUniformNumber()
{
    double number;

    number = std::numbers::pi * (uniform_distribution(generator) - 0.5);  

    return number;
}