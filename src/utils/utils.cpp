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