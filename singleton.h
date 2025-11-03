#pragma once
#include <random>
#include <iostream>

class RNGSingleton {
private:
    std::mt19937 gen;

    // private constructor
    RNGSingleton() {
        std::random_device rd;
        gen.seed(rd());
        std::cout << "RNGSingleton constructor called\n";
    }

    // private destructor
    ~RNGSingleton() {
        std::cout << "RNGSingleton destructor called\n";
    }

    // delete copy/move
    RNGSingleton(const RNGSingleton&) = delete;
    RNGSingleton& operator=(const RNGSingleton&) = delete;

public:
    static RNGSingleton& getInstance() {
        static RNGSingleton instance;
        return instance;
    }

    std::mt19937& engine() { return gen; }
};
