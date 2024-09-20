#include <numbers>
#include "stable_simulation.hpp"


Simulator::Simulator(double alphaInput, double betaInput, double gammaInput, double deltaInput, 
unsigned short parametrizationIndexInput, unsigned int nInput) : 
StableDistribution(alphaInput, betaInput, gammaInput, deltaInput, parametrizationIndexInput), n(Simulator::validateN(nInput))
{
    numbers.resize(n);
};


Simulator::~Simulator()
{
    
}


std::vector<double> Simulator::simulateStableXVector(unsigned int n)
{
    if (n != 1)
        numbers.resize(n);
    auto stableLambda = [this]() -> double { return this->simulateStableX(); };
    std::generate(numbers.begin(), numbers.end(), stableLambda);

    return numbers;
}


std::vector<double> Simulator::simulateSymmetricZVector(unsigned int n)
{
    if (n != 1)
        numbers.resize(n);
    auto symmetricZLambda = [this]() -> double { return this->simulateNonSymmetricZ(); };
    std::generate(numbers.begin(), numbers.end(), symmetricZLambda);

    return numbers;
}


std::vector<double> Simulator::simulateNonSymmetricZVector(unsigned int n)
{
    if (n != 1)
        numbers.resize(n);
    auto nonSymmetricZLambda = [this]() -> double { return this->simulateNonSymmetricZ(); };
    std::generate(numbers.begin(), numbers.end(), nonSymmetricZLambda);

    return numbers;
}


std::vector<double> Simulator::simulateExponentialVector(unsigned int n)
{
    if (n != 1)
        numbers.resize(n);
    auto exponentialLambda = [this]() -> double { return this->simulateExponentialNumber(); };
    std::generate(numbers.begin(), numbers.end(), exponentialLambda);

    return numbers;
}


std::vector<double> Simulator::simulateUniformVector(unsigned int n)
{
    if (n != 1)
        numbers.resize(n);
    auto uniformLambda = [this]() -> double { return this->simulateUniformNumber(); };
    std::generate(numbers.begin(), numbers.end(), uniformLambda);

    return numbers;
}


double Simulator::simulateStableX()
{
    /*

    Can be used to generate both 0 and 1 paramterized variables, even though Z is
    generated using the 1 parametrization. 
    
    */
    double number;
    double Z = simulateNonSymmetricZ();

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


double Simulator::simulateSymmetricZ()
{
    //Generates a symmetric Z with the 1 parametrization. 
    double number;
    double uniform = simulateUniformNumber();

    if (alpha == 1)
    {
        number = tan(uniform);
    }
    else
    { 
        double exponential = simulateExponentialNumber();
        number = (sin(alpha*uniform) / pow(cos(uniform), (1/alpha))) * pow((cos((alpha - 1) * uniform)) / exponential, (1-alpha) / alpha);
    }

    return number;
}


double Simulator::simulateNonSymmetricZ()
{
    //Generates a nonsymmetric Z with the 1 parametrization. 
    double number;
    double uniform = simulateUniformNumber();
    double exponential = simulateExponentialNumber();

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


double Simulator::simulateExponentialNumber()
{
    double number;

    number = -std::log(uniform_distribution(generator));

    return number;
}


double Simulator::simulateUniformNumber()
{
    double number;

    number = std::numbers::pi * (uniform_distribution(generator) - 0.5);  

    return number;
}


unsigned int Simulator::validateN(const unsigned int& n)
{
    if (n == 0)
    {
        throw std::invalid_argument("n must be greater than 0");
    }

    return n;
}