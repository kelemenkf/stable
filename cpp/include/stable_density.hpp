#ifndef STABLE_DENSITY_HPP
#define STABLE_DENSITY_HPP
#include "utils.hpp"
#include "quadrature_rules.hpp"
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <iostream>

class Density
{
private: 
    std::vector<double> X;
    double alpha;
    double beta;
    double gamma; 
    double delta;
    double zeta; 
    int n;
    double T_alpha; 
    double eps; 
    double bound;  
    bool negative;
    std::vector<double> pdf;
    std::vector<double> scaled_nodes;
    std::vector<double> scaled_weights;
    std::vector<double> X_negative;
    std::vector<bool> x_method;


public: 
    Density(std::vector<double> inputX, double alphaInput = 2.0, double betaInput = 0.0,
    double gammaInput = 1.0, double deltaInput = 0.0, bool negativeInput = false, double epsInput = std::numeric_limits<double>::epsilon()) 
    : X(inputX), alpha(alphaInput), beta(betaInput), gamma(gammaInput), delta(deltaInput), negative(negativeInput), eps(epsInput)
    {
        if (alpha != 2.0)
        {
            zeta = calculateZeta();
            if ((gamma != 1.0 || delta != 0 || beta != 0) && !negative)
            {
                std::transform(X.begin(), X.end(), X.begin(), [this](double x) { return (x - delta + zeta * gamma) / gamma; });
            }
            n = determineN();
            T_alpha = calculateTAlpha();
            bound = calculateSeriesBound();
            X_negative = determineXNegative();
            x_method = determineXMethod();
            pdf = calculateDensity();
        }
        else
        {
            pdf = calculateDensity();
        }
    }

    ~Density() {}

    std::vector<double> getPdf()
    {
        if (!negative)
        {
            std::vector<double> scaled_pdf(pdf.size());
            std::transform(pdf.begin(), pdf.end(), scaled_pdf.begin(), [this](double p) { return (1 / gamma) * p; });
            return scaled_pdf;
        }
        else
        {
            return pdf;
        }
    }

    std::vector<double> getScaledNodes()
    {
        return scaled_nodes;
    }

    std::vector<double> getScaledWeights()
    {
        return scaled_weights;
    }

private: 
    double calculateZeta()
    {
        if (beta == 0)
        {
            return 0;
        }
        else
        {
            return -beta * std::tan((M_PI * alpha) / 2);
        }
    }

    int determineN()
    {
        if (beta == 0)
        {
            return 46;
        }
        else if (beta != 0 && alpha >= 1.1)
        {
            return 80;
        }
        else if (beta != 0 && alpha >= 0.5 && alpha <= 0.9)
        {
            return 90;
        }
        return 0;
    }

    std::vector<double> determineXNegative()
    {
        std::vector<double> result;
        if (!negative)
        {
            auto it = std::remove_if(X.begin(), X.end(), [this, &result](double x) {
                if (x - zeta < 0)
                {
                    result.push_back(x);
                    return true;
                }
                return false;
            });
            X.erase(it, X.end());
        }
        return result;
    }

    std::vector<bool> determineXMethod()
    {
        std::vector<bool> method(X.size());
        std::transform(X.begin(), X.end(), method.begin(), [this](double x) { return (x - zeta < bound); });
        return method;
    }

    double calculateSeriesBound()
    {
        return std::pow((alpha / (M_PI * eps) * std::pow(1 + zeta * zeta, n / 2.0) * std::tgamma(alpha * n) / std::tgamma(n)), 1 / (alpha * n - 1));
    }

    double calculateTAlpha()
    {
        return std::pow(-std::log(eps), 1 / alpha);
    }

    void scaleQuadratureRule()
    {
        if (beta == 0)
        {
            scaled_nodes.assign(SYMMETRIC_NODES.begin(), SYMMETRIC_NODES.end());
            scaled_weights.assign(SYMMETRIC_WEIGHTS.begin(), SYMMETRIC_WEIGHTS.end());
            std::transform(scaled_weights.begin(), scaled_weights.end(), scaled_weights.begin(), [this](double w) { return w * (T_alpha / M_PI); });
        }
        else if (beta != 0 && alpha >= 1.1)
        {
            scaled_nodes.assign(ASYMMETRIC_NODES_ALPHA_MORE_11.begin(), ASYMMETRIC_NODES_ALPHA_MORE_11.end());
            scaled_weights.assign(ASYMMETRIC_WEIGHTS_ALPHA_MORE_11.begin(), ASYMMETRIC_WEIGHTS_ALPHA_MORE_11.end());
            std::transform(scaled_weights.begin(), scaled_weights.end(), scaled_weights.begin(), [this](double w) { return w * (T_alpha / M_PI); });
        }
        else if (beta != 0 && alpha <= 0.9 && alpha >= 0.5)
        {
            scaled_nodes.assign(ASYMMETRIC_NODES_ALPHA_LESS_09.begin(), ASYMMETRIC_NODES_ALPHA_LESS_09.end());
            scaled_weights.assign(ASYMMETRIC_WEIGHTS_ALPHA_LESS_09.begin(), ASYMMETRIC_WEIGHTS_ALPHA_LESS_09.end());
            std::transform(scaled_weights.begin(), scaled_weights.end(), scaled_weights.begin(), [this](double w) { return w * (T_alpha / M_PI); });
        }
    }

    double integrandSymmetric(double tau, double x)
    {
        return std::cos(x * (tau * T_alpha)) * std::exp(-std::pow(tau * T_alpha, alpha));
    }

    double integrandAsymmetric(double tau, double x)
    {
        double h = (x - zeta) * (tau * T_alpha) + zeta * std::pow(tau * T_alpha, alpha);
        return std::cos(h) * std::exp(-std::pow(tau * T_alpha, alpha));
    }

    double quadrature(double x)
    {
        if (beta == 0)
        {
            return std::inner_product(scaled_nodes.begin(), scaled_nodes.end(), scaled_weights.begin(), 0.0, std::plus<>(), [this, x](double tau, double w) {
                return integrandSymmetric(tau, x) * w;
            });
        }
        else
        {
            return std::inner_product(scaled_nodes.begin(), scaled_nodes.end(), scaled_weights.begin(), 0.0, std::plus<>(), [this, x](double tau, double w) {
                return integrandAsymmetric(tau, x) * w;
            });
        }
    }

    double seriesRepresentation(double x)
    {
        double f_x = 0;
        for (int k = 1; k <= n; ++k)
        {
            double first_term = std::pow(-1, k + 1);
            double second_term = std::tgamma(alpha * k) / std::tgamma(k);
            double third_term = std::pow(1 + zeta * zeta, k / 2.0);
            double fourth_term = std::sin((M_PI * alpha / 2 - std::atan(zeta)) * k);
            double fifth_term = std::pow(x - zeta, -alpha * k - 1);
            f_x += first_term * second_term * third_term * fourth_term * fifth_term;
        }
        return (alpha / M_PI) * f_x;
    }

    std::vector<double> calculateDensity()
    {
        std::vector<double> pdf;
        if (alpha == 2.0)
        {
            std::transform(X.begin(), X.end(), std::back_inserter(pdf), [this](double x) {
                return std::exp(-std::pow((x - delta) / (gamma * std::sqrt(2)), 2)) / (gamma * std::sqrt(2 * M_PI));
            });
        }
        else
        {
            scaleQuadratureRule();
            if (!negative)
            {
                std::transform(X_negative.begin(), X_negative.end(), X_negative.begin(), [this](double x) {
                    return -x;
                });
                Density negative_density(X_negative, alpha, -beta, gamma, delta, true);
                std::vector<double> negative_pdf = negative_density.getPdf();
                pdf.insert(pdf.end(), negative_pdf.begin(), negative_pdf.end());
            }
            for (size_t i = 0; i < X.size(); ++i)
            {
                if (x_method[i])
                {
                    pdf.push_back(quadrature(X[i]));
                }
                else
                {
                    pdf.push_back(seriesRepresentation(X[i]));
                }
            }
        }
        return pdf;
    }
};

#endif