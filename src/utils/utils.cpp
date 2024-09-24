#include "utils.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

double getQuantile(const std::vector<double>& sample, const float& p)
{
    size_t index = (sample.size() * p) - 1;

    return sample[index];
}


double linearInterpolation(CartesianPoint point0, CartesianPoint point1, double x)
{
    return (point0[1] * (point1[0] - x) + point1[1] * (x - point0[0])) / (point1[0] - point0[0]);
}


std::vector<double> splitString(const std::string& str, const std::string& delimiter) {
    std::vector<double> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(std::stod(str.substr(start, end - start)));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(std::stod(str.substr(start)));

    return tokens;
}