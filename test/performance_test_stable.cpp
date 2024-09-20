#include <chrono>
#include "stable_simulation.hpp"
#include <string>
#include <iostream>
using namespace std::chrono;


void timeExecutionExponential(Simulator* const stable, unsigned int n)
{
    auto start = high_resolution_clock::now();

    stable->simulateExponentialVector(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates exponential vector at " << duration.count() << std::endl;
}


void timeExecutionUniform(Simulator* const stable, unsigned int n)
{
    auto start = high_resolution_clock::now();

    stable->simulateUniformVector(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates uniform vector at " << duration.count() << std::endl;
}


void timeExecutionSymmetricZ(Simulator* const stable, unsigned int n)
{
    auto start = high_resolution_clock::now();

    stable->simulateSymmetricZVector(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates symmetric standardized stable vector at " << duration.count() << std::endl;
}


void timeExecutionNonSymmetricZ(Simulator* const stable, unsigned int n)
{
    auto start = high_resolution_clock::now();

    stable->simulateNonSymmetricZVector(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates nonsymmetric standardized vector at " << duration.count() << std::endl;
}


void timeExecutionStable(Simulator* const stable, unsigned int n)
{
    auto start = high_resolution_clock::now();

    stable->simulateStableXVector(n);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    std::cout << "Generates stable vector at " << duration.count() << std::endl;
}


int main()
{
    Simulator* stable = new Simulator(1.5, 0.5, 2, 4);

    unsigned int n = 10000;

    timeExecutionExponential(stable, n);

    timeExecutionUniform(stable, n);

    timeExecutionSymmetricZ(stable, n);

    timeExecutionNonSymmetricZ(stable, n);

    timeExecutionStable(stable, n);

    delete stable;

    return 0;
}
