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
        std::cout << uniform_distribution(generator) << std::endl;
        number = -std::log(uniform_distribution(generator));

        return number;
    }
};


int main()
{
    using std::cout, std::cin, std::endl;

    StableDistribution* stable = new StableDistribution();

    std::cout << stable->generateExponentialNumber() << endl;
    std::cout << stable->generateUniformNumber() << endl;

    return 0;
}