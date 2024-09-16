#include <iostream>
#include <chrono>
#include "stable.hpp"
using namespace std::chrono;

int main()
{
    using std::cout, std::cin, std::endl;

    StableDistribution* stable = new StableDistribution();

    auto start = high_resolution_clock::now();

    stable->generateExponentialVector();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start);

    cout << "Geneates exponential vector at " << duration.count() << endl;

    return 0;
}