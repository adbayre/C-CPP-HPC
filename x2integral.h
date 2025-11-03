#pragma once
#include "MonteCarloIntegralComputer.h"
// non abstract constructor class
class X2Integral : public MonteCarloIntegralComputer {
public:
    X2Integral(double a_, double b_) : MonteCarloIntegralComputer(a_, b_) {}

    double f(double x) const override {
        return x * x;
    }
};