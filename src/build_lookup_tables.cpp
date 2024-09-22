#include "stable_distribution.hpp"
#include "quantile_estimator_lookup_table.hpp"

int main(int argc, char* argv[])
{
    QuantileEstimatorLookupTable table(0.1, 0.5, 2.0, 0.0, 1.0);
    table.calculateLookupTables();
    table.writeLookupTablesToFile("vAlpha");
    table.writeLookupTablesToFile("vBeta");
    table.writeLookupTablesToFile("vGamma");
    table.writeLookupTablesToFile("vDelta");

    return 0;
}