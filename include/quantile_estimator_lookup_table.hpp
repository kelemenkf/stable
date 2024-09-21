#ifndef QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP
#define QUANTILE_ESTIMATOR_LOOKUP_TABLE_HPP

#include "stable_simulation.hpp"
#include <string>
#include <map>

class QuantileEstimatorLookupTable
{
private:
    std::map<std::string, std::vector<double>> table;
    double mesh;


public:
    QuantileEstimatorLookupTable(double meshInput);

    void writeLookupTableToFile();
};

#endif