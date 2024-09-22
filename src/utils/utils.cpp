#include "utils.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

double getQuantile(const std::vector<double>& sample, const float& p)
{
    size_t index = sample.size() * p - 1;

    return sample[index];
}