#include <cstdlib>  
#include "stable_distribution.hpp"
#include "quantile_estimator_lookup_table.hpp"

int main(int argc, char* argv[])
{   
    if (argc < 2)
    {
        QuantileEstimatorLookupTable table(0.1, 0.5, 2.0);
        table.calculateLookupTables(10);
        table.writeLookupTablesToFile("vAlpha");
        table.writeLookupTablesToFile("vBeta");
        table.writeLookupTablesToFile("vGamma");
        table.writeLookupTablesToFile("vDelta");
    }
    else
    {
        unsigned int numberOfSamples = static_cast<unsigned int>(std::stoul(argv[1]));
        double mesh =  static_cast<double>(std::stof(argv[2]));
        double alphaMin = static_cast<double>(std::stof(argv[3]));
        double alphaMax = static_cast<double>(std::stof(argv[4]));
        double betaMin = static_cast<double>(std::stof(argv[5]));
        double betaMax = static_cast<double>(std::stof(argv[6]));
        
        QuantileEstimatorLookupTable table(mesh, alphaMin, alphaMax, betaMin, betaMax);
        table.calculateLookupTables(numberOfSamples);
        table.writeLookupTablesToFile("vAlpha");
        table.writeLookupTablesToFile("vBeta");
        table.writeLookupTablesToFile("vGamma");
        table.writeLookupTablesToFile("vDelta");
    }

    return 0;
}