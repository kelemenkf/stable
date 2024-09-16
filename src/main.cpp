#include <iostream>
#include <chrono>
#include "stable.hpp"
using namespace std::chrono;

int main()
{
    using std::cout, std::cin, std::endl;

    StableDistribution* stable = new StableDistribution();

    std::cout << stable->getParametrization() << std::endl;

    return 0;
}