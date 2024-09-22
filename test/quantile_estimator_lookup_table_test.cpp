#define BOOST_TEST_MODULE QuantileEstimatorLookupTableTestSuite
#include <boost/test/included/unit_test.hpp>
#include <filesystem>
#include "quantile_estimator_lookup_table.hpp"

struct LookupTableFixture: public QuantileEstimatorLookupTable
{
    LookupTableFixture() {};

    LookupTableFixture(double meshInput, double alphaMinInput, double alphaMaxInput, double betaMinInput, 
    double betaMaxInput) : QuantileEstimatorLookupTable(meshInput, alphaMinInput, alphaMaxInput, betaMinInput, betaMaxInput) {};

    ~LookupTableFixture() {};

    double testGetMesh() const {
        return getMesh();
    }

    double testGetAlphaMin() const {
        return getAlphaMin();
    }

    double testGetAlphaMax() const {
        return getAlphaMax();
    }

    double testGetBetaMin() const {
        return getBetaMin();
    }

    double testGetBetaMax() const {
        return getBetaMax();
    }
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


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorTableCalculationDefaultInitialization, LookupTableFixture ) {
    QuantileEstimatorLookupTable table;

    BOOST_CHECK_EQUAL(getMesh(), 0.1);
    BOOST_CHECK_EQUAL(getAlphaMin(), 0.1); 
    BOOST_CHECK_EQUAL(getAlphaMax(), 2.0);
    BOOST_CHECK_EQUAL(getBetaMin(), 0.0);
    BOOST_CHECK_EQUAL(getBetaMax(), 1.0);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableMemberTableInitialziation, LookupTableFixture) {
    QuantileEstimatorLookupTable table;

    size_t table_size;
    size_t expected_size = 4;
    table_size = getTableSize();

    BOOST_CHECK_EQUAL(table_size, expected_size); 
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVAlphaSize, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    table.calculateLookupTables();

    size_t vAlphaSize;
    size_t expectedSize = 220;
    vAlphaSize = table["vAlpha"].size();

    BOOST_CHECK_EQUAL(vAlphaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVBetaSize, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    table.calculateLookupTables();

    size_t vBetaSize;
    size_t expectedSize = 220;
    vBetaSize = table["vBeta"].size();

    BOOST_CHECK_EQUAL(vBetaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVGammaSize, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    table.calculateLookupTables();

    size_t vGammaSize;
    size_t expectedSize = 220;
    vGammaSize = table["vGamma"].size();

    BOOST_CHECK_EQUAL(vGammaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVDeltaSize, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    table.calculateLookupTables();

    size_t vDeltaSize;
    size_t expectedSize = 220;
    vDeltaSize = table["vDelta"].size();

    BOOST_CHECK_EQUAL(vDeltaSize, expectedSize);
}


BOOST_AUTO_TEST_CASE( QuantileEstimatorTableNonDefaultInitialization ) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);

    BOOST_CHECK_EQUAL(fixtureTable.testGetMesh(), 0.25);
    BOOST_CHECK_EQUAL(fixtureTable.testGetAlphaMin(), 0.5); 
    BOOST_CHECK_EQUAL(fixtureTable.testGetAlphaMax(), 2.0);
    BOOST_CHECK_EQUAL(fixtureTable.testGetBetaMin(), 0.5);
    BOOST_CHECK_EQUAL(fixtureTable.testGetBetaMax(), 1.0);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVAlphaNonDefaultSize, LookupTableFixture) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);
    fixtureTable.calculateLookupTables();

    size_t vAlphaSize;
    size_t expectedSize = ((fixtureTable.testGetAlphaMax() - fixtureTable.testGetAlphaMin() + fixtureTable.testGetMesh())/ fixtureTable.testGetMesh()) *
    ((fixtureTable.testGetBetaMax() - fixtureTable.testGetBetaMin() + fixtureTable.testGetMesh()) / fixtureTable.testGetMesh());
    vAlphaSize = fixtureTable["vAlpha"].size();

    BOOST_CHECK_EQUAL(vAlphaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVBetaNonDefaultSize, LookupTableFixture) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);
    fixtureTable.calculateLookupTables();

    size_t vBetaSize;
    size_t expectedSize = ((fixtureTable.testGetAlphaMax() - fixtureTable.testGetAlphaMin() + fixtureTable.testGetMesh())/ fixtureTable.testGetMesh()) *
    ((fixtureTable.testGetBetaMax() - fixtureTable.testGetBetaMin() + fixtureTable.testGetMesh()) / fixtureTable.testGetMesh());
    vBetaSize = fixtureTable["vBeta"].size();

    BOOST_CHECK_EQUAL(vBetaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVGammaNonDefaultSize, LookupTableFixture) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);
    fixtureTable.calculateLookupTables();

    size_t vGammaSize;
    size_t expectedSize = ((fixtureTable.testGetAlphaMax() - fixtureTable.testGetAlphaMin() + fixtureTable.testGetMesh())/ fixtureTable.testGetMesh()) *
    ((fixtureTable.testGetBetaMax() - fixtureTable.testGetBetaMin() + fixtureTable.testGetMesh()) / fixtureTable.testGetMesh());
    vGammaSize = fixtureTable["vGamma"].size();

    BOOST_CHECK_EQUAL(vGammaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableCalculateVDeltaNonDefaultSize, LookupTableFixture) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);
    fixtureTable.calculateLookupTables();

    size_t vDeltaSize;
    size_t expectedSize = ((fixtureTable.testGetAlphaMax() - fixtureTable.testGetAlphaMin() + fixtureTable.testGetMesh())/ fixtureTable.testGetMesh()) *
    ((fixtureTable.testGetBetaMax() - fixtureTable.testGetBetaMin() + fixtureTable.testGetMesh()) / fixtureTable.testGetMesh());
    vDeltaSize = fixtureTable["vDelta"].size();

    BOOST_CHECK_EQUAL(vDeltaSize, expectedSize);
}


BOOST_FIXTURE_TEST_CASE( QuantileEstimatorLookupTableWriteToFileSuccess, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    std::string vFunction = "vAlpha";
    table.writeLookupTablesToFile(vFunction);

    std::string filePath = "/assets/" + vFunction + "_lookup_tables.csv";

    BOOST_CHECK(std::filesystem::exists(filePath));
}


BOOST_AUTO_TEST_SUITE_END()