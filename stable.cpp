#include <iostream>
#include <random>
#include <vector>
#include <numbers>
#include <cmath>

class StableDistribution
{
private:
    double alpha; 
    double beta;
    double gamma; 
    double delta; 
    int parametrization_index;
    std::mt19937 generator; 
    std::uniform_real_distribution<double> uniform_distribution;

public: 
    StableDistribution(): generator(12345), uniform_distribution(0, 1) {};
    
    StableDistribution(double alphaInput, double betaInput, double gammaInput,
    double deltaInput) : alpha(alphaInput), beta(betaInput), gamma(gammaInput), 
    delta(deltaInput), generator(std::random_device{}()), uniform_distribution(0, 1) {};

    ~StableDistribution()
    {

    }

    void getUniformBounds() const
    {
        auto params = uniform_distribution.param();
        std::cout << params.a() << std::endl;
        std::cout << params.b() << std::endl;
    }

    double generateUniformNumber()
    {
        double number;
        number = std::numbers::pi * (uniform_distribution(generator) - 0.5);  

        return number;
    }

    double generateExponentialNumber()
    {
        double number;
        number = -std::log(uniform_distribution(generator));

        return number;
    }

    double generateSymmetricStableNumber()
    {
        double number;
        double uniform = generateUniformNumber();

        if (alpha == 1)
        {
            number = tan(uniform);

            return number;
        }
        else
        { 
            double exponential = generateExponentialNumber();
            number = (sin(alpha * uniform) / pow(cos(uniform), (1/alpha))) * pow(((cos((alpha - 1) * uniform))/ exponential), ((1-alpha)/alpha));

            return number;
        }
    }

    double generateNonSymmetricStableNumber()
    {
        double number;
        double uniform = generateUniformNumber();
        double exponential = generateExponentialNumber();

        if (alpha == 1)
        {
            number = (2/std::numbers::pi) * (((std::numbers::pi/2) + (beta*uniform)) * tan(uniform) - beta * log(((std::numbers::pi/2 * exponential * cos(uniform)))/((std::numbers::pi/2) + beta*uniform)));

            return number;
        }     
        else
        {
            double theta_0 = atan(beta*tan(std::numbers::pi*alpha/2)) / alpha;
            number = (sin(alpha * (theta_0 + uniform)) / pow(cos(alpha*theta_0) * cos(uniform), (1/alpha))) * pow(((cos(alpha*theta_0 + (alpha - 1) * uniform))/ exponential), ((1-alpha)/alpha));

            return number;
        }
    }
};


int main()
{
    using std::cout, std::cin, std::endl;

    StableDistribution* stable = new StableDistribution(1.5,0,1,0);

    cout << stable->generateExponentialNumber() << endl;
    cout << stable->generateUniformNumber() << endl;

    cout << "Symmetric stable: " <<  stable->generateSymmetricStableNumber() << endl;
    cout << "Nonsymmetric stable: " << stable->generateNonSymmetricStableNumber() << endl;

    return 0;
}