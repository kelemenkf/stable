#include "utils.hpp"
#include <vector>
#include <algorithm>

double getQuantile(const std::vector<double>& sample, const float& p)
{
    size_t index = sample.size() * p;

    return sample[index];
}