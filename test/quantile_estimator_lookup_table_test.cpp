#define BOOST_TEST_MODULE QuantileEstimatorLookupTableTestSuite
#include <boost/test/included/unit_test.hpp>
#include <filesystem>
#include <ranges>
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

    int testGetSampleCounterForTest() const {
        return getSampleCounterForTest();
    }

    std::vector<double> testCalculateSumOfSamples(const unsigned int& numberOfSamples)
    {
        return calculateSumOfSamples(numberOfSamples);
    }

    std::vector<double> testCalculateMeanOfSamples(std::vector<double>& sumOfSamples)
    {
        return calculateMeanOfSamples(sumOfSamples);
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


// BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableCalculateDefaultSize, LookupTableFixture) {
//     QuantileEstimatorLookupTable table;
//     size_t expectedSize = 220;
//     table.calculateLookupTables();

//     size_t vAlphaSize;
//     vAlphaSize = table["vAlpha"].size();
//     BOOST_CHECK_EQUAL(vAlphaSize, expectedSize);

//     size_t vBetaSize;
//     vBetaSize = table["vBeta"].size();
//     BOOST_CHECK_EQUAL(vBetaSize, expectedSize);

//     size_t vGammaSize;
//     vGammaSize = table["vGamma"].size();
//     BOOST_CHECK_EQUAL(vGammaSize, expectedSize);

//     size_t vDeltaSize;
//     vDeltaSize = table["vDelta"].size();
//     BOOST_CHECK_EQUAL(vDeltaSize, expectedSize);
// }


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorTableNonDefaultInitialization ) {
    LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);

    BOOST_CHECK_EQUAL(fixtureTable.testGetMesh(), 0.25);
    BOOST_CHECK_EQUAL(fixtureTable.testGetAlphaMin(), 0.5); 
    BOOST_CHECK_EQUAL(fixtureTable.testGetAlphaMax(), 2.0);
    BOOST_CHECK_EQUAL(fixtureTable.testGetBetaMin(), 0.5);
    BOOST_CHECK_EQUAL(fixtureTable.testGetBetaMax(), 1.0);
}


// BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableCalculateNonDefaultSize, LookupTableFixture) {
//     LookupTableFixture fixtureTable(0.25, 0.5, 2.0, 0.5, 1.0);
//     fixtureTable.calculateLookupTables();
//     size_t expectedSize = ((fixtureTable.testGetAlphaMax() - fixtureTable.testGetAlphaMin() + fixtureTable.testGetMesh())/ fixtureTable.testGetMesh()) *
//     ((fixtureTable.testGetBetaMax() - fixtureTable.testGetBetaMin() + fixtureTable.testGetMesh()) / fixtureTable.testGetMesh());

//     size_t vAlphaSize;
//     vAlphaSize = fixtureTable["vAlpha"].size();
//     BOOST_CHECK_EQUAL(vAlphaSize, expectedSize);

//     size_t vBetaSize;
//     vBetaSize = fixtureTable["vBeta"].size();
//     BOOST_CHECK_EQUAL(vBetaSize, expectedSize);

//     size_t vGammaSize;
//     vGammaSize = fixtureTable["vGamma"].size();
//     BOOST_CHECK_EQUAL(vGammaSize, expectedSize);

//     size_t vDeltaSize;
//     vDeltaSize = fixtureTable["vDelta"].size();
//     BOOST_CHECK_EQUAL(vDeltaSize, expectedSize);
// }


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableWriteToFileSuccessVAlpha, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    std::string vFunction = "vAlpha";
    table.writeLookupTablesToFile(vFunction);

    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";

    BOOST_CHECK(std::filesystem::exists(filePath));
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableWriteToFileSuccessVBeta, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    std::string vFunction = "vBeta";
    table.writeLookupTablesToFile(vFunction);

    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";

    BOOST_CHECK(std::filesystem::exists(filePath));
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableWriteToFileSuccessVGamma, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    std::string vFunction = "vGamma";
    table.writeLookupTablesToFile(vFunction);

    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";

    BOOST_CHECK(std::filesystem::exists(filePath));
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableWriteToFileSuccessVDelta, LookupTableFixture) {
    QuantileEstimatorLookupTable table;
    std::string vFunction = "vDelta";
    table.writeLookupTablesToFile(vFunction);

    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";

    BOOST_CHECK(std::filesystem::exists(filePath));
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableNumberLengthOfSample, LookupTableFixture) {
    LookupTableFixture fixtureTable;
    unsigned int numberOfSamples = 1;
    std::vector<double> sumOfSamples = fixtureTable.testCalculateSumOfSamples(numberOfSamples);

    BOOST_CHECK_EQUAL(sumOfSamples.size(), 10000);
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableNumberOfSamplesUsed, LookupTableFixture) {
    LookupTableFixture fixtureTable;
    unsigned int numberOfSamples = 100;
    std::vector<double> sumOfSamples = fixtureTable.testCalculateSumOfSamples(numberOfSamples);

    BOOST_CHECK_EQUAL(fixtureTable.testGetSampleCounterForTest(), numberOfSamples);
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableSumOfSamplesElements, LookupTableFixture) {
    LookupTableFixture fixtureTable;
    unsigned int numberOfSamples = 1;
    std::vector<double> sumOfSamples = fixtureTable.testCalculateSumOfSamples(numberOfSamples);

    bool isNonZero = std::any_of(sumOfSamples.begin(), sumOfSamples.end(), [](double value){ return value !=0 ; });

    BOOST_CHECK(isNonZero);
}


BOOST_FIXTURE_TEST_CASE( TestQuantileEstimatorLookupTableMeanOfSamplesUesd, LookupTableFixture) {
    LookupTableFixture fixtureTable;
    unsigned int numberOfSamples = 100;
    std::vector<double> sumOfSamples = fixtureTable.testCalculateSumOfSamples(100);
    std::vector<double> meanOfSamples = fixtureTable.testCalculateMeanOfSamples(sumOfSamples);

    auto meanView = sumOfSamples | std::views::transform([numberOfSamples](double val){
        return val / numberOfSamples;
    });

    std::vector<double> meanViewAsVector(meanView.begin(), meanView.end());

    BOOST_CHECK_EQUAL_COLLECTIONS(meanOfSamples.begin(), meanOfSamples.end(), meanViewAsVector.begin(), 
    meanViewAsVector.end());
}


BOOST_AUTO_TEST_SUITE_END()