#define BOOST_TEST_MODULE QuantileEstimatorLookupTableTestSuite
#include <boost/timer/timer.hpp>
#include <filesystem>
#include "quantile_estimator_lookup_table.hpp"
#include <iostream>


int main()
{
    boost::timer::cpu_timer timer;

    QuantileEstimatorLookupTable table;

    std::cout << "Elapsed time: " << timer.format() << std::endl;
}