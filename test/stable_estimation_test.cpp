#define BOOST_TEST_MODULE StableEstimationTestSuite
#include <boost/test/included/unit_test.hpp>
#include "stable_estimation.hpp"

BOOST_AUTO_TEST_SUITE( StableEstimationTestSuite )

BOOST_AUTO_TEST_CASE( EstimatorConstructor ) {
    std::vector<double> sample;
    BOOST_CHECK_THROW(Estimator estimator(sample), std::invalid_argument);

    sample = {1, 2, 45, 6, 6, 7};
    BOOST_CHECK_NO_THROW(Estimator estmator(sample));
}

BOOST_AUTO_TEST_SUITE_END()