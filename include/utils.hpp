#include <vector>
#include <algorithm>

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
};

double getQuantile(const std::vector<double>& sample, const float& p);

double linearInterpolation(CartesianPoint point1, CartesianPoint poin2, double x);
