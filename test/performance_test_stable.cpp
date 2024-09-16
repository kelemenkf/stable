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

    std::cout << "Generates exponential vector at " << duration.count() << std::endl;
}


int main()
{
    StableDistribution* stable = new StableDistribution();

    timeExecutionExponential(stable);

    timeExecutionUniform(stable);

    delete stable;

    return 0;
}
