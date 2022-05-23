#pragma once

#include <SFML/System.hpp>
#include <array>
#include <cmath>

namespace constants {
    static constexpr float g = 9.8;
}

enum class Layer {
    Bg,
    Character,
    Hitbox,
    Bullet,
    Interface
};

enum class Tag {
    None,
    PlayerBullet,
    Player,
    Enemy,
    Bullet,
    Wall
};

enum class HitboxType {
    Circle,
    Rect
};

static constexpr sf::Uint8 layer_num = 5;

inline std::array<Layer, layer_num> Layers = {Layer::Bg, Layer::Character, Layer::Bullet, Layer::Hitbox, Layer::Interface};

template <typename T>
inline T squared_distance(sf::Vector2<T> a, sf::Vector2<T> b) {
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

template <typename T>
inline T max(sf::Vector2<T> a) {
    return a.x > a.y ? a.x : a.y;
}

template <typename T>
inline T min(sf::Vector2<T> a) {
    return a.x < a.y ? a.x : a.y;
}

template <typename T>
inline T sign(T x) {
    return (x > 0) - (x < 0);
}

template <typename T>
std::ostream& operator<<(std::ostream &out, sf::Vector2<T> vec) {
    out << vec.x << ", " << vec.y;
    return out;
}

// static const sf::Vector2f left(-1.0, 0.0);
static const sf::Vector2f right(1.0, 0.0);
// static const sf::Vector2f up(0.0, -1.0);
static const sf::Vector2f down(0.0, 1.0);
