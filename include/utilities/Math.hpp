#pragma once

#include <SFML/System.hpp>
#include <array>
#include <cmath>

namespace constants {
    inline constexpr float pi = 3.1415;
}

inline float to_radians(float deg) {
    return deg / 180 * 2 * constants::pi;
}

inline float to_degrees(float rad) {
    return rad * 180 / 2 / constants::pi;
}

template <typename T>
inline T tan(sf::Vector2<T> a) {
    return a.x / a.y;
}

template <typename T>
inline T arctan(sf::Vector2<T> a) {
    return atan(tan(a));
}

template <typename T>
inline T abs(sf::Vector2<T> a) {
    return distance(a, sf::Vector2<T>{0, 0});
}

template <typename T>
inline T squared_distance(sf::Vector2<T> a, sf::Vector2<T> b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

template <typename T>
inline T distance(sf::Vector2<T> a, sf::Vector2<T> b) {
    return sqrt(squared_distance(a, b));
}

template <typename T>
inline sf::Vector2<T> unit_vector(sf::Vector2<T> a, sf::Vector2<T> b) {
    return (a - b) / distance(a, b);
}

template <typename T>
inline sf::Vector2<T> unit_vector(sf::Vector2<T> a) {
    return a / abs(a);
}
