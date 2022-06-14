#pragma once

template <typename T>
inline T max(sf::Vector2<T> a) {
    return a.x > a.y ? a.x : a.y;
}

template <typename T>
inline T min(sf::Vector2<T> a) {
    return a.x < a.y ? a.x : a.y;
}

template <typename T>
inline T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
inline T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T>
inline T sign(T x) {
    return (x > 0) - (x < 0);
}

template <typename T>
inline std::ostream& operator<<(std::ostream &out, sf::Vector2<T> vec) {
    out << vec.x << "," << vec.y;
    return out;
}

template <typename T>
inline sf::Vector2<T> operator*(sf::Vector2<T> a, sf::Vector2<T> b) {
    return {a.x * b.x, a.y * b.y};
}

inline const sf::Vector2f right(1.0, 0.0);
inline const sf::Vector2f down(0.0, 1.0);