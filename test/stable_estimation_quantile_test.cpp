#define BOOST_TEST_MODULE StableEstimationQuantileTestSuite
#include <boost/test/included/unit_test.hpp>
#include "stable_estimation_quantile.hpp"

BOOST_AUTO_TEST_SUITE( StableEstimationQuantileTestSuite )

BOOST_AUTO_TEST_CASE( QuantileEstimatorConstructor ) {
    std::vector<double> sample;
    BOOST_CHECK_THROW(QuantileEstimator estimator(sample), std::invalid_argument);

    sample = {1.0, 2.33, 45.942, 6.1, 6.2, 7.6};
    BOOST_CHECK_NO_THROW(QuantileEstimator estmator(sample));
}

BOOST_AUTO_TEST_SUITE_END()