#define BOOST_TEST_MODULE StableEstimationQuantileTestSuite
#include <boost/test/included/unit_test.hpp>
#include "stable_estimation_quantile.hpp"


struct QuantileEstimatorFixture: public QuantileEstimator
{
    QuantileEstimatorFixture(std::vector<double> sampleInput) : QuantileEstimator(sampleInput) {};

    ~QuantileEstimatorFixture() {};

    void testCalculateVAlpha()
    {
        calculateVAlpha();
    }

    double testGetVAlpha() 
    {
        return getVAlpha();
    }

    void testCalculateVBeta()
    {
        calculateVBeta();
    }

    double testGetVBeta() 
    {
        return getVBeta();
    }

    std::vector<double> testGetSample()
    {
        return getSample();
    }

    void testCalculateQVector()
    {
        calculateQVector();
    }

    std::vector<double> testGetSampleQs()
    {
        return getSampleQs();
    }

    std::vector<CartesianPoint> testFindAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector)
    {
        return findAdjacentQuantiles(quantile, xVector, yVector);
    }

    std::vector<double> testGetCorrectedQuantiles()
    {
        return getCorrectedQuantiles();
    }


    std::pair<double, double> testEstimateAlphaBeta()
    {
        return estimateAlphaBeta();
    }
};


BOOST_AUTO_TEST_SUITE( StableEstimationQuantileTestSuite )


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorConstructor ) {
    std::vector<double> testSample;
    BOOST_CHECK_THROW(QuantileEstimator testEstimator(testSample), std::invalid_argument);

    testSample = {1.0, 2.33, 45.942, 6.1, 6.2, 7.6};
    BOOST_CHECK_NO_THROW(QuantileEstimator testEstmator(testSample));
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorConstructorSorting ) {
    std::vector<double> testSample = {1.0, 2.33, 45.942, 6.1, 6.2, 7.6};
    QuantileEstimatorFixture testEstimator(testSample);

    std::sort(testSample.begin(), testSample.end(), [](double a, double b){ return a < b; });
    std::vector<double> testInputSample = testEstimator.testGetSample();

    BOOST_CHECK_EQUAL_COLLECTIONS(testInputSample.begin(), testInputSample.end()
    , testSample.begin(), testSample.end());
}


BOOST_AUTO_TEST_CASE( TestQunatileEstimatorCalculateVAlphaSample ) {
    std::vector<double> testSample(100);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });

    QuantileEstimatorFixture testEstimator(testSample);
    testEstimator.testCalculateVAlpha();
    BOOST_CHECK_EQUAL(testEstimator.testGetVAlpha(), (9.0 / 5));
}


BOOST_AUTO_TEST_CASE( TestQunatileEstimatorCalculateVBetaSample ) {
    std::vector<double> testSample(100);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });

    QuantileEstimatorFixture testEstimator(testSample);
    testEstimator.testCalculateVBeta();
    BOOST_CHECK_EQUAL(testEstimator.testGetVBeta(), 0);
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorCalculateQVector ) {
    std::vector<double> testSample(100);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });
    QuantileEstimatorFixture estimator(testSample);

    estimator.testCalculateQVector();
    std::vector<double> testSampleQs = estimator.testGetSampleQs();

    BOOST_CHECK_GT(testSampleQs[0], 0);
    BOOST_CHECK_LT(testSampleQs[testSampleQs.size() - 1], 1);
}


BOOST_AUTO_TEST_CASE( TestQunatileEstimatorFindAdjacentQuantilesEvenSample ) {
    std::vector<double> testSample(100);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });
    QuantileEstimatorFixture estimator(testSample);

    estimator.testCalculateQVector();
    std::vector<CartesianPoint> testPoints = estimator.testFindAdjacentQuantiles(0.500, estimator.testGetSampleQs(), estimator.testGetSample());

    BOOST_CHECK_EQUAL(testPoints[0][0], 0.495);
    BOOST_CHECK_EQUAL(testPoints[1][0], 0.505);
    BOOST_CHECK_EQUAL(testPoints[0][1], 49);
    BOOST_CHECK_EQUAL(testPoints[1][1], 50);
}

BOOST_AUTO_TEST_CASE( TestQunatileEstimatorFindAdjacentQuantilesOddSample ) {
    std::vector<double> testSample(101);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });
    QuantileEstimatorFixture estimator(testSample);

    estimator.testCalculateQVector();
    std::vector<CartesianPoint> testPoints = estimator.testFindAdjacentQuantiles(0.500, estimator.testGetSampleQs(), estimator.testGetSample());

    BOOST_CHECK_EQUAL(testPoints[0][0], 0.5);
    BOOST_CHECK_EQUAL(testPoints[1][0], 0.5);
    BOOST_CHECK_EQUAL(testPoints[0][1], 50);
    BOOST_CHECK_EQUAL(testPoints[1][1], 50);
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorReadInLookupTableFile ) {
    std::vector<double> testSample(101);
    int start = 0;
    std::generate(testSample.begin(), testSample.end(), [&start]() mutable { return start++; });
    QuantileEstimatorFixture estimator(testSample);
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorFindAdjacentAlphasInRange ) {
    Simulator simulator(1.1, 0.7);

    std::vector<double> alphas;
    for (size_t i = 0; i < 100; ++i)
    {
        std::vector<double> testSample = simulator.simulateStableXVector(10000);
        QuantileEstimatorFixture testEstimator(testSample);
        std::pair<double, double> params =  testEstimator.testEstimateAlphaBeta();
        alphas.push_back(std::get<0>(params));
    }

    double sum = std::accumulate(alphas.begin(), alphas.end(), 0.0);
    double mean = sum / alphas.size();

    std::cout << mean << std::endl;
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorFindAdjacentBetasInRange ) {
    Simulator simulator(1.1, 0.7);

    std::vector<double> betas;
    for (size_t i = 0; i < 100; ++i)
    {
        std::vector<double> testSample = simulator.simulateStableXVector(10000);
        QuantileEstimatorFixture testEstimator(testSample);
        std::pair<double, double> params =  testEstimator.testEstimateAlphaBeta();
        betas.push_back(std::get<1>(params));
    }

    double sum = std::accumulate(betas.begin(), betas.end(), 0.0);
    double mean = sum / betas.size();

    std::cout << mean << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()