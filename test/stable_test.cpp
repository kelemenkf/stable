#define BOOST_TEST_MODULE StableDistributionTestSuite
#include <boost/test/included/unit_test.hpp>
#include "stable_distribution.hpp"

BOOST_AUTO_TEST_SUITE( StableDistributionTestSuite )

BOOST_AUTO_TEST_CASE( AlphaValidation ) {
    double alpha = 1.5;
    BOOST_CHECK_NO_THROW(StableDistribution distribution(alpha));

    alpha = 3.0;
    BOOST_CHECK_THROW(StableDistribution distribution(alpha), std::invalid_argument);

    alpha = -1.0;
    BOOST_CHECK_THROW(StableDistribution distribution(alpha), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE( BetaValidation ) {
    double beta = 0.5;
    BOOST_CHECK_NO_THROW(StableDistribution distribution(1, beta));

    beta = 3.0;
    BOOST_CHECK_THROW(StableDistribution distribution(1, beta), std::invalid_argument);

    beta = -2.0;
    BOOST_CHECK_THROW(StableDistribution distribution(1, beta), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()