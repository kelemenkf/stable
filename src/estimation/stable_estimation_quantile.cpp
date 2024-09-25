#include "stable_estimation_quantile.hpp"
#include <iostream>
#include <fstream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQsInput, std::vector<double>
correctedQuantilesInput) 
: Estimator(sampleInput), sampleQs(sampleQsInput), correctedQuantiles(correctedQuantilesInput) {
    for (std::string vFunction : {"vAlpha", "vBeta", "vGamma", "vDelta"})
    {
        readLookupTableFromFile(vFunction);
    }
    sortSample();
    calculateQVector();
    initializeMemberQuantiles();
    calculateAlpha();
};


QuantileEstimator::~QuantileEstimator()
{

}



double QuantileEstimator::calculateAlpha()
{
    std::vector<CartesianPoint> adjacentPoints;

    adjacentPoints = findAdjacentAlphas(vAlphaSample, lookupTable["vAlpha"]);

    std::cout << adjacentPoints[0].getX() <<  adjacentPoints[1].getX() <<  adjacentPoints[0].getY()
    <<  adjacentPoints[1].getY() << std::endl;
}


std::vector<CartesianPoint> QuantileEstimator::findAdjacentAlphas(double vValue, std::map<std::tuple<double, double>, double> vAlpha)
{
    CartesianPoint before;
    CartesianPoint after;

    auto findQuantile = [&vValue, &before, &after](auto element) mutable {
        if (vValue == element.second)
        {
            before.setX(std::get<0>(element.first));
            after.setX(std::get<0>(element.first));  
            before.setY(element.second);
            after.setY(element.second);
        }
    };

    for_each(vAlpha.begin(), vAlpha.end(), findQuantile);

    auto first = vAlpha.begin();
    auto last = --vAlpha.end();

    if (first->second < vValue)
    {
        before.setX(std::get<0>(first->first));
        after.setX(std::get<0>(first->first));
        before.setY(0.5);
        after.setY(0.5);
    }
    else if (last->second > vValue)
    {   
        before.setX(std::get<0>(last->first));
        after.setX(std::get<0>(last->first));
        before.setY(2.0);
        after.setY(2.0);
    }


    for (auto element = ++vAlpha.begin(); element != --vAlpha.end();
    ++element)
    {
        std::cout << element->second << std::endl;
        auto previous = std::prev(element);
        if (previous->second < vValue &&  element->second > vValue)
        {
            before.setX(previous->second);
            after.setX(element->second);
            before.setY(std::get<0>(element->first));
            after.setY(std::get<0>(element->first));
        }
    }

    std::vector<CartesianPoint> adjacentPoints{before, after};
    return adjacentPoints;
}


void QuantileEstimator::readLookupTableFromFile(const std::string& vFunction)
{
    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";
    std::ifstream lookupTableFile(filePath, std::ios::in);


    std::map<std::tuple<double, double>, double> table = {};
    std::vector<double> alphaValues;
    std::vector<double> betaValues;
    std::vector<double> vValues;

    std::unordered_map<double, double> result;

    std::string line;
    int counter = 0;

    if (lookupTableFile.is_open())
    {
        while(getline(lookupTableFile, line))
        {
            if (counter == 0)
            { 
                betaValues = splitString(line.substr(2), ",");
            }
            else
            {
                std::vector<double> doubleConvertedLine = splitString(line, ",");
                alphaValues.push_back(doubleConvertedLine[0]);
                doubleConvertedLine.erase(doubleConvertedLine.begin());
                vValues.insert(vValues.end(), doubleConvertedLine.begin(), doubleConvertedLine.end());   
            }
            ++counter;
        }
    }    

    for (size_t alphaIndex = 0; alphaIndex < alphaValues.size();
    ++alphaIndex)
    {
        for (size_t betaIndex = 0; betaIndex < betaValues.size();
        ++betaIndex)
        {
            std::tuple<double, double> index {alphaValues[alphaIndex], betaValues[betaIndex]};

            table[index] = vValues[alphaIndex * betaValues.size() + betaIndex];
        }
    }

    lookupTable[vFunction] = table;
}


void QuantileEstimator::sortSample()
{
    std::sort(sample.begin(), sample.end(), [](double a, double b){ return a < b; });
}


void QuantileEstimator::calculateQVector()
{
    sampleQs.resize(sample.size());

    int counter = 1;
    auto fillQVector = [this, &counter]() mutable { 
        return (2.0*(counter++) - 1) / (2 * this->sampleQs.size()); 
    };

    std::generate(sampleQs.begin(), sampleQs.end(), fillQVector);
}


void QuantileEstimator::initializeMemberQuantiles()
{
    std::vector<double> quantilesNeeded {0.05, 0.25, 0.5, 0.75, 0.95};
    for (size_t index = 0; index < quantilesNeeded.size(); ++index)
    {
        correctedQuantiles[index] = calculateCorrectedQuantile(quantilesNeeded[index]);
    }
}


double QuantileEstimator::calculateCorrectedQuantile(const double& quantile)
{
    std::vector<CartesianPoint> adjacentQuantiles = findAdjacentQuantiles(quantile, sampleQs, sample);

    return linearInterpolation(adjacentQuantiles[0], adjacentQuantiles[1], quantile);
} 


std::vector<CartesianPoint> QuantileEstimator::findAdjacentQuantiles(double quantile, std::vector<double> xVector, std::vector<double> yVector)
{
    CartesianPoint before;
    CartesianPoint after;

    size_t counter = 0;
    auto findQuantile = [yVector, &quantile, &counter, &before, &after](double value) mutable {
        if (quantile == value)
        {
            before.setX(value);
            after.setX(value);  
            before.setY(yVector[counter]);
            after.setY(yVector[counter]);
        }
        ++counter;
    };

    for_each(sampleQs.begin(), sampleQs.end(), findQuantile);

    for (size_t index = 0; index < sampleQs.size(); index++)
    {
        if (sampleQs[index - 1] < quantile && sampleQs[index] > quantile)
        {
            before.setX(xVector[index - 1]);
            after.setX(xVector[index]);
            before.setY(yVector[index - 1]);
            after.setY(yVector[index]);
        }
    }

    std::vector<CartesianPoint> adjacentPoints{before, after};
    return adjacentPoints;
}


void QuantileEstimator::calculateVAlpha()
{
    vAlphaSample = (correctedQuantiles[4] - correctedQuantiles[0]) / 
    (correctedQuantiles[3] - correctedQuantiles[1]);
}


void QuantileEstimator::calculateVBeta()
{
    vBetaSample = (correctedQuantiles[0] + correctedQuantiles[4] - 2*correctedQuantiles[2]) / 
    (correctedQuantiles[4] - correctedQuantiles[0]);
}


double QuantileEstimator::getVAlpha()
{
    return vAlphaSample;
}


double QuantileEstimator::getVBeta()
{
    return vBetaSample;
}


std::vector<double> QuantileEstimator::getSample()
{
    return sample;
}


std::vector<double> QuantileEstimator::getSampleQs()
{
    return sampleQs;
}