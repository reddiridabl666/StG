#pragma once

#include <SFML/System.hpp>

enum class Layer {
    bg,
    character,
    hitbox,
    bullet,
    interface
};

static constexpr sf::Uint8 layer_num = 5;

inline std::array<Layer, layer_num> Layers = {Layer::bg, Layer::character, Layer::hitbox, Layer::bullet, Layer::interface};

static const sf::Vector2f left(-1.0, 0.0);
static const sf::Vector2f right(1.0, 0.0);
static const sf::Vector2f up(0.0, -1.0);
static const sf::Vector2f down(0.0, 1.0);
