#pragma once

#include <random>

int rand_gen(int a, int b) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution uni_dist(a, b);
    return uni_dist(gen);
}

float rand_gen(float a, float b) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution uni_dist(a, b);
    return uni_dist(gen);
}
