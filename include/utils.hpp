#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

class CartesianPoint
{
private:
    double x;
    double y;

public: 
    CartesianPoint() {};

    CartesianPoint(double xInput, double yInput) : x(xInput), y(yInput) {};

    ~CartesianPoint() {};

    double operator[] (unsigned int index)
    {
        if (index == 0)
        {
            return x;
        }
        else if (index == 1)
        {
            return y;
        }
    }

    void setX(double input)
    {
        x = input;
    }

    void setY(double input)
    {
        y = input;
    }
};

double getQuantile(const std::vector<double>& sample, const float& p);

double linearInterpolation(CartesianPoint point1, CartesianPoint poin2, double x);

std::vector<double> splitString(const std::string& str, const std::string& delimiter);

template<typename T>
void displayVector(const std::vector<T>& vector)
{
    for (auto element = vector.cbegin(); element != vector.cend(); 
    ++element)
    {
        std::cout << *element << " " << std::endl;
    }
}

template<typename T>
std::vector<T> vectorExtensionWithItself(int times, std::vector<T> inputVector)
{   
    std::vector<T> original = inputVector;
    for (size_t i = 1; i < times; ++i)
    {
        inputVector.insert(inputVector.end(), original.begin(), original.end());
    }

    return inputVector;
}


template<typename T>
std::map<T, T> buildMapFromVectors(std::vector<T> keyVector, std::vector<T> valueVector)
{
    std::map<T, T> result;

    if (keyVector.size() == valueVector.size())
    {
        for (size_t index = 0; index < keyVector.size(); ++index)
        {
            result[keyVector[index]] = valueVector[index];
        }
    }
}

#endif
