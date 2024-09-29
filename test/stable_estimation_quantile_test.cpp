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


    std::pair<double, double> testEstimateAlpha()
    {
        return estimateAlpha();
    }

    std::pair<double, double> testEstimateBeta()
    {
        return estimateBeta();
    }
    
    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> testFindAlphaPoints()
    {
        return findAlphaPoints();
    }

    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> testFindBetaPoints(
        std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints)
    {
        return findBetaPoints(alphaPoints);
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


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorFindAlphaPoints ) {
    Simulator simulator(1.15, 0.75);

    const double epsilon = 1e-6; 
    int searchCounter = 0;
    std::vector<std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator>> alphaPointVector;
    for (size_t i = 0; i < 100; ++i)
    {
        std::vector<double> testSample = simulator.simulateStableXVector(10000);
        QuantileEstimatorFixture testEstimator(testSample);
        std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints = testEstimator.testFindAlphaPoints();
        if (std::fabs(std::get<0>(alphaPoints)->alpha - 1.1) < epsilon && 
            std::fabs(std::get<1>(alphaPoints)->alpha - 1.2) < epsilon) 
        {
            ++searchCounter;
        }
        alphaPointVector.push_back(alphaPoints);
    }

    BOOST_CHECK_GE(searchCounter, 95);
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorFindBetaPoints ) {
    Simulator simulator(1.15, 0.55);

    const double epsilon = 1e-6;
    int searchCounter = 0;
    std::vector<std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator>> betaPointVector;
    for (size_t i = 0; i < 100; ++i)
    {
        std::vector<double> testSample = simulator.simulateStableXVector(10000);
        QuantileEstimatorFixture testEstimator(testSample);
        std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints = testEstimator.testFindAlphaPoints();
        std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> betaPoints = testEstimator.testFindBetaPoints(alphaPoints);
        if (std::fabs(std::get<1>(betaPoints)->beta - 0.6) < epsilon && 
            std::fabs(std::get<0>(betaPoints)->beta - 0.5) < epsilon) 
        {
            ++searchCounter;
        }
        betaPointVector.push_back(betaPoints);
    }


    BOOST_CHECK_GE(searchCounter, 95);
}


BOOST_AUTO_TEST_CASE( TestQuantileEstimatorFindAdjacentAlphasInRange ) {
    std::vector<double> alphaTestValues;
    std::vector<double> betaTestValues;
    double alphaStart = 0.6;
    double betaStart = 0.1;

    while (alphaStart <= 1.9)
    {
        alphaTestValues.push_back(alphaStart); 
        alphaStart += 0.05;
    }
    while (betaStart <= 0.9)
    {
        betaTestValues.push_back(betaStart);
        betaStart += 0.05;
    }

    for (size_t alphaIndex = 0; alphaIndex < alphaTestValues.size(); ++alphaIndex)
    {
        for (size_t betaIndex = 0; betaIndex < betaTestValues.size(); ++betaIndex)
        {
            Simulator simulator(alphaTestValues[alphaIndex], betaTestValues[betaIndex]);

            std::vector<double> alphas;
            std::vector<double> betas;
            for (size_t i = 0; i < 100; ++i)
            {
                std::vector<double> testSample = simulator.simulateStableXVector(10000);
                QuantileEstimatorFixture testEstimator(testSample);
                std::pair<double, double> alpha =  testEstimator.testEstimateAlpha();
                std::pair<double, double> beta = testEstimator.testEstimateBeta();
                alphas.push_back(std::get<0>(alpha));
                betas.push_back(std::get<1>(beta));
            }
            double alphaSum = std::accumulate(alphas.begin(), alphas.end(), 0.0);
            double alphaMean = alphaSum / alphas.size();

            BOOST_CHECK_GE(alphaMean, alphaTestValues[alphaIndex] - 0.025);
            BOOST_CHECK_LE(alphaMean, alphaTestValues[alphaIndex] + 0.025);

            double betaSum = std::accumulate(betas.begin(), betas.end(), 0.0);
            double betaMean = betaSum / betas.size();

            BOOST_CHECK_GE(betaMean, betaTestValues[betaIndex] - 0.025);
            BOOST_CHECK_LE(betaMean, betaTestValues[betaIndex] + 0.025);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()