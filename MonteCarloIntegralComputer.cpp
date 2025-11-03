#include "MonteCarloIntegralComputer.h"
#include <cmath>
#include <algorithm>

MonteCarloIntegralComputer::MonteCarloIntegralComputer(double a_, double b_)
    : n(0), a(a_), b(b_), sumSamples(0.0), sumSquares(0.0) {}

void MonteCarloIntegralComputer::compute(int nb_points) {
    auto& rng = RNGSingleton::getInstance().engine();
    std::uniform_real_distribution<double> dist(a, b);

    for (int i = 0; i < nb_points; ++i) {
        double x = dist(rng);
        double fx = f(x);
        sumSamples += fx;
        sumSquares += fx * fx;
        ++n;
    }
}

double MonteCarloIntegralComputer::operator()() const {
    if (n == 0) return 0.0;
    double mean = sumSamples / n;
    return (b - a) * mean;
}

pair<double,double> MonteCarloIntegralComputer::confidenceInterval(double z) const {
    if (n == 0) return {0.0, 0.0};

    double mean = sumSamples / n;
    double meanSquare = sumSquares / n;
    double variance = meanSquare - mean * mean;
    variance = max(variance, 0.0);

    double stdError = sqrt(variance / n);
    double margin = z * stdError;

    double estimate = (b - a) * mean;
    double delta = (b - a) * margin;

    return {estimate - delta, estimate + delta};
}
