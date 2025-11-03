#pragma once
#include <utility>
#include "Singleton.h"


using namespace std;
class MonteCarloIntegralComputer {
    protected:
        long long n; // Number of points
        double a, b; // Bounds
        double sumSamples; // Sum of f(x)
        double sumSquares; // Sum of f(x)^2

    public:
        MonteCarloIntegralComputer(double a_, double b_);
        virtual ~MonteCarloIntegralComputer() = default;
        virtual double f(double x) const = 0;
        void compute(int nb_points);
        double operator()() const;
        pair<double,double> confidenceInterval(double z = 1.96) const;

        long long samples() const { return n; }
};