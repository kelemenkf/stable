#define BOOST_TEST_MODULE QuantileEstimatorLookupTableTestSuite
#include <boost/test/included/unit_test.hpp>
#include "quantile_estimator_lookup_table.hpp"

struct LookupTableFixture: public QuantileEstimatorLookupTable
{
    LookupTableFixture() {};
    ~LookupTableFixture() {};
};

BOOST_AUTO_TEST_SUITE( QuantileEstimatorLookupTableTestSuite )


BOOST_AUTO_TEST_CASE( QuantileEstimatorLookupTableConstructorValidation ) {
    BOOST_CHECK_NO_THROW(QuantileEstimatorLookupTable table(0.1));
    BOOST_CHECK_THROW(QuantileEstimatorLookupTable table(1.5), std::invalid_argument);

    BOOST_CHECK_NO_THROW(QuantileEstimatorLookupTable table(0.1, 1));
    BOOST_CHECK_THROW(QuantileEstimatorLookupTable table(0.1, -1.0), std::invalid_argument);

    BOOST_CHECK_NO_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2));
    BOOST_CHECK_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2.5), std::invalid_argument);

    BOOST_CHECK_NO_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2, 0.5));
    BOOST_CHECK_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2, -0.5), std::invalid_argument);

    BOOST_CHECK_NO_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2, 0.5, 1));
    BOOST_CHECK_THROW(QuantileEstimatorLookupTable table(0.1, 1, 2, 0.5, 1.5), std::invalid_argument);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorTableCalculationSize, LookupTableFixture ) {
    QuantileEstimatorLookupTable table;
    table.calculateLookupTable();
    double size = ((getAlphaMax() - getAlphaMin()) / getMesh()) * ((getBetaMax() / getBetaMin()) / getMesh());

    BOOST_CHECK_EQUAL(getTableSize(), static_cast<size_t>(size)); 
}


BOOST_AUTO_TEST_SUITE_END()