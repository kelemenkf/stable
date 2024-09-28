#include "stable_estimation_quantile.hpp"
#include <iostream>
#include <fstream>

QuantileEstimator::QuantileEstimator(std::vector<double> sampleInput, std::vector<double> sampleQsInput, std::vector<double>
correctedQuantilesInput) 
: Estimator(sampleInput), sampleQs(sampleQsInput), correctedQuantiles(correctedQuantilesInput) {
    populateTableAlphaBeta();
    for (std::string vFunction : {"vAlpha", "vBeta"})
    {
        readLookupTableFromFile(vFunction);
    }
    std::reverse(lookupTable.begin(), lookupTable.end());
    sortSample();
    calculateQVector();
    initializeMemberQuantiles();
    calculateVAlpha();
    calculateVBeta();
};


QuantileEstimator::~QuantileEstimator()
{

}


std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> QuantileEstimator::findAlphaPoints()
{
    // Find the four closest points in the table
    auto compareVAlpha = [](const TableEntry& entry, double value) {
        return entry.vAlpha < value;
    };
    auto upperAlpha = std::lower_bound(lookupTable.begin(), lookupTable.end(), vAlphaSample, compareVAlpha);
    
    if (upperAlpha == lookupTable.begin()) upperAlpha++;
    if (upperAlpha == lookupTable.end()) upperAlpha--;

    auto lowerAlpha = upperAlpha;
    upperAlpha--;

    return {lowerAlpha, upperAlpha};
}


std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> QuantileEstimator::findBetaPoints(
    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints)
{
    auto startBeta = std::get<0>(alphaPoints) - 10;
    auto endBeta = std::get<1>(alphaPoints) + 10;

    if (endBeta == lookupTable.end()) {
        std::cerr << "Error: Could not find startBeta or endBeta." << std::endl;
    }

    auto upperBeta = std::lower_bound(startBeta, std::get<1>(alphaPoints), vBetaSample, 
        [this](const TableEntry& entry, double value) {
            return entry.vBeta > value;
        });
    auto lowerBeta = upperBeta + 1;


    return {lowerBeta, upperBeta};
}



std::pair<double, double> QuantileEstimator::estimateAlphaBeta()
{
    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> alphaPoints = findAlphaPoints();
    std::pair<std::vector<TableEntry>::iterator, std::vector<TableEntry>::iterator> betaPoints = findBetaPoints(alphaPoints);

    auto lowerAlpha = std::get<0>(alphaPoints);
    auto upperAlpha = std::get<1>(alphaPoints);
    auto lowerBeta = std::get<0>(betaPoints);
    auto upperBeta = std::get<1>(betaPoints);

    lowerAlpha->beta = lowerBeta->beta;
    upperAlpha->alpha = lowerAlpha ->alpha;
    upperAlpha->beta = upperBeta->beta;

    double x = (vAlphaSample - lowerAlpha->vAlpha) / (upperAlpha->vAlpha - lowerAlpha->vAlpha);
    double y = (vBetaSample - lowerBeta->vBeta) / (upperBeta->vBeta - lowerBeta->vBeta);

    // Bilinear interpolation for alpha
    double alpha = (1 - x) * (1 - y) * lowerAlpha->alpha +  // bottom-left
                   x * (1 - y) * upperAlpha->alpha +        // bottom-right
                   (1 - x) * y * lowerBeta->alpha +         // top-left
                   x * y * upperBeta->alpha;                // top-right

    // Bilinear interpolation for beta
    double beta = (1 - x) * (1 - y) * lowerAlpha->beta +   // bottom-left
                  x * (1 - y) * upperAlpha->beta +         // bottom-right
                  (1 - x) * y * lowerBeta->beta +          // top-left
                  x * y * upperBeta->beta;                 // top-right 

    return {alpha, beta};
}


void QuantileEstimator::populateTableAlphaBeta()
{
    double mesh = 0.1;
    double alphaMin = 0.5;
    double alphaMax = 2.0;
    double betaMin = 0;
    double betaMax = 1.0;

    std::vector<double> alphaValues;
    fillVector(alphaValues, mesh, alphaMin, alphaMax);

    std::vector<double> betaValues;
    fillVector(betaValues, mesh, betaMin, betaMax);

    for (size_t i = 0; i < alphaValues.size(); ++i)
    {
        for(size_t j = 0; j < betaValues.size(); ++j)
        {
            lookupTable.push_back(TableEntry(alphaValues[i], betaValues[j], 0, 0));
        }
    }   
}


void QuantileEstimator::fillVector(std::vector<double>& parameters, double& mesh, double& paramMin, double& paramMax)
{
    parameters.resize(static_cast<size_t>((paramMax - paramMin + mesh) / mesh));

    double currentParam = paramMin;

    auto vectorFill = [paramMax, mesh, &currentParam]() mutable -> double {
        double param = currentParam;
        
        if (currentParam <= paramMax)
        {
            currentParam += mesh;
        } 

        return param;
    };

    std::generate(parameters.begin(), parameters.end(), vectorFill);
}


void QuantileEstimator::readLookupTableFromFile(const std::string& vFunction)
{
    std::string filePath = "../../assets/" + vFunction + "_lookup_tables.csv";
    std::ifstream lookupTableFile(filePath, std::ios::in);

    std::vector<double> alphaValues;
    std::vector<double> betaValues;
    std::vector<double> vValues;

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
            if (vFunction == "vAlpha")
            {
                lookupTable[alphaIndex * betaValues.size() + betaIndex].setVAlpha(vValues[alphaIndex * betaValues.size() + betaIndex]);
            }
            else if (vFunction == "vBeta")
            {
                lookupTable[alphaIndex * betaValues.size() + betaIndex].setVBeta(vValues[alphaIndex * betaValues.size() + betaIndex]);
            }
        }
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


std::vector<double> QuantileEstimator::getCorrectedQuantiles()
{
    return correctedQuantiles;
}