#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include "complexNumber.h"
#include "x2integral.h"
using namespace std;

// Pi computation from exercise 1
void computePi(){
    random_device rd; // To obtain a seed for the random number engine
    mt19937 gen(rd());
    long long count = 0;
    const long long n_iter = 10'000'000;
    uniform_real_distribution<> dis(-1.0, 1.0);
    vector<double> x(n_iter), y(n_iter);

    #pragma omp parallel for reduction(+:count)
    for (long long n = 0; n < n_iter; ++n){
        
        x[n] = dis(gen);
        y[n] = dis(gen);
        
    }
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < n_iter; ++i) {        
        count+= (x[i]*x[i] + y[i]*y[i] <= 1.0);
    }
        
    double pi = (count * 4.0) / n_iter;
    cout << "An approximation of pi is " << pi ;
}

// Integral computation from exercise 2
void computeX2Integral(){
    random_device rd;
    mt19937 gen(rd());
    long long count = 0;
    const long long n_iter = 10'000'000;
    uniform_real_distribution<> dis(0.0, 3.0);
    uniform_real_distribution<> hig(0.0, 9.0);
    vector<double> x(n_iter), y(n_iter);
    #pragma omp parallel for reduction(+:count)
    for (long long n = 0; n < n_iter; ++n){        
        x[n] = dis(gen);
        y[n] = hig(gen);
    }
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < n_iter; ++i) {        
        count+= (x[i]*x[i] > y[i]);
    }
    double integral = (count*27.0 / n_iter);
    cout << "An approximation of the integral is " << integral ;

}

// Pi computation from exercise 4
double computeComplexPi(long long n_points, mt19937* ptr_gen){
    long long count = 0;
    ComplexNumber point = ComplexNumber(0,0);
    uniform_real_distribution<float> dist(0.0, 1.0);
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < n_points; i++){
        point.Set(dist(*ptr_gen), dist(*ptr_gen));
        count+= (point.abs() <= 1.0);
    }
    double pi = (count * 4.0) / n_points;
    return pi;
}

int main(){
    //computePi();
    //computeX2Integral();
    // random_device rd;
    // mt19937 gen(rd());
    // cout << "An approximation of pi is " << computeComplexPi(10000000, &gen) << endl;

    X2Integral integral1(0.0, 1.0);
    integral1.compute(1000000);
    cout << "Estimate (integral1): " << integral1() << "\n";

    auto ci1 = integral1.confidenceInterval();
    cout << "95% Confidence Interval integral1: [" << ci1.first << ", " << ci1.second << "]\n";

    X2Integral integral2(0.0, 1.0);
    integral2.compute(500000);
    cout << "Estimate integral2: " << integral2() << "\n";

    auto ci2 = integral2.confidenceInterval();
    cout << "95% Confidence Interval integral2: [" << ci2.first << ", " << ci2.second << "]\n";
    return 0;
}