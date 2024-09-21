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


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorTableCalculationDefaultSize, LookupTableFixture ) {
    QuantileEstimatorLookupTable table;
    size_t table_size;
    size_t expected_size = 220;
    table_size = getTableSize();

    BOOST_CHECK_EQUAL(getAlphaMax(), 2.0);
    BOOST_CHECK_EQUAL(getAlphaMin(), 0.1); 
    BOOST_CHECK_EQUAL(getBetaMax(), 1.0);
    BOOST_CHECK_EQUAL(getBetaMin(), 0.0);
    BOOST_CHECK_EQUAL(getMesh(), 0.1);

    BOOST_CHECK_EQUAL(table_size, expected_size); 
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorTableCalculationSize, LookupTableFixture ) {
    QuantileEstimatorLookupTable table(0.25, 0.5, 2.0, 0.5, 1.0);
    size_t table_size;
    size_t expected_size;
    table_size = getTableSize();
    expected_size = (getAlphaMax() / getMesh()) *  ((getBetaMax() + getMesh()) / getMesh());

    BOOST_CHECK_EQUAL(getAlphaMax(), 2.0);
    BOOST_CHECK_EQUAL(getAlphaMin(), 0.5); 
    BOOST_CHECK_EQUAL(getBetaMax(), 1.0);
    BOOST_CHECK_EQUAL(getBetaMin(), 0.5);
    BOOST_CHECK_EQUAL(getMesh(), 0.25);

    BOOST_CHECK_EQUAL(table_size, expected_size); 
}



BOOST_AUTO_TEST_SUITE_END()