#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
#include "stable.hpp"
#include <numbers>

BOOST_AUTO_TEST_CASE( test_stable_distribution )
{
    StableDistribution stable;
    BOOST_TEST( stable.generateUniformNumber() >= -(std::numbers::pi) / 2);
    BOOST_TEST( stable.generateUniformNumber() <= (std::numbers::pi) / 2 );
}
