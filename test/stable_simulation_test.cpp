#define BOOST_TEST_MODULE StableSimulationTestSuite
#include <boost/test/included/unit_test.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#include "stable_simulation.hpp"
#include <numbers>
#include <algorithm>


class Variance
{
private:
    double mean;
    double sse;
    int n;

public: 
    Variance(double meanInput) : mean(meanInput), sse(0), n(0) {};

    void operator() (double num)
    {
        sse += pow((num - mean), 2);
        n += 1;
    }

    double getVariance()
    {
        return (sse / (n-1)); 
    }
};


double getChiSquaredUpperCriticalValue(double alpha, size_t df)
{
    return boost::math::quantile(boost::math::chi_squared(df), alpha / 2);
}


double getChiSquaredLowerCriticalValue(double alpha, size_t df)
{
    return boost::math::quantile(boost::math::chi_squared(df), 1 - (alpha / 2));
}


double getVarianceTolerance(double variance, size_t df, double criticalValue)
{
    return ((df-1) * variance) / criticalValue;
}


BOOST_AUTO_TEST_SUITE( StableSimulationTestSuite )


BOOST_AUTO_TEST_CASE( SimulatorConstruction ) {

}


BOOST_AUTO_TEST_CASE( TestUniformSize ) {
    Simulator simulator;

    std::vector<double> numbers = simulator.generateUniformVector(10000);
    BOOST_CHECK_EQUAL(numbers.size(), 10000);

    for (auto num: numbers) {
        BOOST_CHECK_GE(num, -std::numbers::pi / 2);
        BOOST_CHECK_LE(num, std::numbers::pi / 2);
    }
}


BOOST_AUTO_TEST_CASE( TestUniformBounds ) {
    Simulator simulator;

    std::vector<double> numbers = simulator.generateUniformVector(10000);

    for (auto num: numbers) {
        BOOST_CHECK_GE(num, -std::numbers::pi / 2);
        BOOST_CHECK_LE(num, std::numbers::pi / 2);
    }
}


BOOST_AUTO_TEST_CASE( TestUniformMean ) {
    Simulator simulator;
    std::vector<double> numbers = simulator.generateUniformVector(10000);

    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    double mean = sum / numbers.size();
    Variance varianceAccumulator(mean);
    std::for_each(numbers.begin(), numbers.end(), std::ref(varianceAccumulator));
    double variance = varianceAccumulator.getVariance();

    double standardError = std::sqrt(variance) / std::sqrt(numbers.size());
    double meanTolerance = 1.96 * standardError;

    BOOST_CHECK_GE(mean, 0 - meanTolerance);
    BOOST_CHECK_LE(mean, 0 + meanTolerance);
}


BOOST_AUTO_TEST_CASE( TestUniformVariance ) {
    Simulator simulator;
    std::vector<double> numbers = simulator.generateUniformVector(10000);

    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    double mean = sum / numbers.size();
    Variance varianceAccumulator(mean);
    std::for_each(numbers.begin(), numbers.end(), std::ref(varianceAccumulator));
    double variance = varianceAccumulator.getVariance();

    double alpha = 0.05;
    double lowerCriticalValue = getChiSquaredLowerCriticalValue(alpha, numbers.size() - 1);
    double upperCriticalValue = getChiSquaredUpperCriticalValue(alpha, numbers.size() - 1);
    double varianceUpperTolerance = getVarianceTolerance(variance, numbers.size() - 1, upperCriticalValue);
    double varianceLowerTolerance = getVarianceTolerance(variance, numbers.size() - 1, lowerCriticalValue);

    BOOST_CHECK_GE(variance, varianceLowerTolerance);
    BOOST_CHECK_LE(variance, varianceUpperTolerance);
}


BOOST_AUTO_TEST_CASE( TestExponentialSize ) {
    Simulator simulator;

    std::vector<double> numbers = simulator.generateExponentialVector(10000);
    BOOST_CHECK_EQUAL(numbers.size(), 10000);
}


BOOST_AUTO_TEST_CASE( TestExponentialBounds ) {
    Simulator simulator;

    std::vector<double> numbers = simulator.generateExponentialVector(10000);

    for (auto num: numbers) {
        BOOST_CHECK_GE(num, 0);
    }
}


BOOST_AUTO_TEST_CASE( TestExponentialMean ) {
    Simulator simulator;
    std::vector<double> numbers = simulator.generateExponentialVector(10000);

    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    double mean = sum / numbers.size();
    Variance varianceAccumulator(mean);
    std::for_each(numbers.begin(), numbers.end(), std::ref(varianceAccumulator));
    double variance = varianceAccumulator.getVariance();

    double standardError = std::sqrt(variance) / std::sqrt(numbers.size());
    double meanTolerance = 1.96 * standardError;

    BOOST_CHECK_GE(mean, 1 - meanTolerance);
    BOOST_CHECK_LE(mean, 1 + meanTolerance);
}


BOOST_AUTO_TEST_CASE( TestExponentialVariance ) {
    Simulator simulator;
    std::vector<double> numbers = simulator.generateExponentialVector(10000);

    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    double mean = sum / numbers.size();
    Variance varianceAccumulator(mean);
    std::for_each(numbers.begin(), numbers.end(), std::ref(varianceAccumulator));
    double variance = varianceAccumulator.getVariance();

    double alpha = 0.05;
    double lowerCriticalValue = getChiSquaredLowerCriticalValue(alpha, numbers.size() - 1);
    double upperCriticalValue = getChiSquaredUpperCriticalValue(alpha, numbers.size() - 1);
    double varianceLowerTolerance = getVarianceTolerance(variance, numbers.size() - 1, lowerCriticalValue);
    double varianceUpperTolerance = getVarianceTolerance(variance, numbers.size() - 1, upperCriticalValue);

    BOOST_CHECK_GE(variance, varianceLowerTolerance);
    BOOST_CHECK_LE(variance, varianceUpperTolerance);
}


BOOST_AUTO_TEST_CASE( StableSimulation )
{
    Simulator simulator;
    std::vector<double> numbers = simulator.generateSymmetricZVector(1000);
    BOOST_CHECK_EQUAL(numbers.size(), 1000);


    numbers = simulator.generateNonSymmetricZVector(1000);
    BOOST_CHECK_EQUAL(numbers.size(), 1000);


    numbers = simulator.generateStableXVector(1000);
    BOOST_CHECK_EQUAL(numbers.size(), 1000);
} 


BOOST_AUTO_TEST_SUITE_END()