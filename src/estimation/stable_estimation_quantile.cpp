#include "stable_estimation_quantile.hpp"
#include <iostream>
#include <fstream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQsInput, std::vector<double>
correctedQuantilesInput) 
: Estimator(sampleInput), sampleQs(sampleQsInput), correctedQuantiles(correctedQuantilesInput) {
    for (std::string parameter: {"alpha", "beta"})
    {
        invertTable(parameter);
    }
    sortSample();
    calculateQVector();
    initializeMemberQuantiles();
};


QuantileEstimator::~QuantileEstimator()
{

}


void QuantileEstimator::invertTable(const std::string& parameter)
{
    std::unordered_map<double, double> vAlpha_param = readLookupTableFromFile("vAlpha", parameter);
    std::unordered_map<double, double> vBeta_param = readLookupTableFromFile("vBeta", parameter);

    std::map<std::tuple<double, double>, double> table;

    for (auto elementA = vAlpha_param.begin(); elementA != vAlpha_param.end();
    ++elementA)
    {
        for (auto elementB = vBeta_param.begin(); elementB != vBeta_param.end();
        ++elementB)
        {
            std::tuple<double, double> index{elementA->second, elementB->second};

            table[index] = elementA->first;
        }
    }

    invertedTable[parameter] = table;
}


std::map<std::string, std::map<std::tuple<double, double>, double>> readlookupTableFromFile(std::string vFunction)
{
    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";
    std::ifstream lookupTableFile(filePath, std::ios::in);


    std::map<std::string, std::map<std::tuple<double, double>, double>> lookupTable = 
    {
        {vFunction, {}} 
    };
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

            lookupTable[vFunction][index] = vValues[alphaIndex * betaValues.size() + betaIndex];
        }
    }

    return lookupTable;
}



std::unordered_map<double, double> QuantileEstimator::readLookupTableFromFile(std::string vFunction, const std::string& parameter)
{
    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";

    std::ifstream lookupTableFile(filePath, std::ios::in);

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

    if (parameter == "alpha")
    {
        alphaValues = vectorExtensionWithItself(11, alphaValues);
        roundDoubleVector(vValues, 3);
        result = buildMapFromVectors(vValues, alphaValues);

        for (auto element = result.begin(); element != result.end();
        ++element)
        {
            std::cout << element->first << " , "  << element->second << std::endl;
        }

        return result;
    }
    else if (parameter == "beta")
    {
        betaValues = vectorExtensionWithItself(16, betaValues);
        roundDoubleVector(vValues, 3);
        transposeVector(vValues, 16, 11);
        result = buildMapFromVectors(vValues, betaValues);

        return result;
    }

    else
    {
        std::cerr << "Not a valid parameter" << std::endl;
    }
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


std::map<std::tuple<double, double>, double> QuantileEstimator::getInvertedTable(const std::string& parameter)
{
    return invertedTable[parameter];
}