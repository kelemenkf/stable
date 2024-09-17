#include <chrono>
#include "stable.hpp"
#include <string>
#include <iostream>
using namespace std::chrono;


void timeExecutionExponential(StableDistribution* const stable)
{
    auto start = high_resolution_clock::now();

    stable->generateExponentialVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates exponential vector at " << duration.count() << std::endl;
}


void timeExecutionUniform(StableDistribution* const stable)
{
    auto start = high_resolution_clock::now();

    stable->generateUniformVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates uniform vector at " << duration.count() << std::endl;
}


void timeExecutionSymmetricZ(StableDistribution* const stable)
{
    auto start = high_resolution_clock::now();

    stable->generateSymmetricZVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates symmetric standardized stable vector at " << duration.count() << std::endl;
}


void timeExecutionNonSymmetricZ(StableDistribution* const stable)
{
    auto start = high_resolution_clock::now();

    stable->generateNonSymmetricZVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates nonsymmetric standardized vector at " << duration.count() << std::endl;
}


void timeExecutionStable(StableDistribution* const stable)
{
    auto start = high_resolution_clock::now();

    stable->generateStableXVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates stable vector at " << duration.count() << std::endl;
}


int main()
{
    StableDistribution* stable = new StableDistribution(1.5, 0.5, 2, 4);

    timeExecutionExponential(stable);

    timeExecutionUniform(stable);

    timeExecutionSymmetricZ(stable);

    timeExecutionNonSymmetricZ(stable);

    timeExecutionStable(stable);

    delete stable;

    return 0;
}
