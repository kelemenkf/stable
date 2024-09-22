#include <iostream>
#include "stable_distribution.hpp"
#include "quantile_estimator_lookup_table.hpp"

int main()
{
    using std::cout, std::cin, std::endl;

    QuantileEstimatorLookupTable table;
    table.calculateLookupTables();
    table.writeLookupTablesToFile("vAlpha");

    return 0;
}