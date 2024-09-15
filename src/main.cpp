#include <iostream>
#include "stable.hpp"

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