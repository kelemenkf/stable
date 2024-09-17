#include <iostream>
#include "stable_distribution.hpp"

int main()
{
    using std::cout, std::cin, std::endl;

    StableDistribution* stable = new StableDistribution();

    std::cout << stable->getParametrization() << std::endl;

    return 0;
}