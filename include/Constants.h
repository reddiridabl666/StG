#pragma once

#include <SFML/Graphics.hpp>

enum class Layer {
    bg,
    character,
    hitbox,
    bullet,
};

inline std::array<Layer, 4> Layers = {Layer::bg, Layer::character, Layer::hitbox, Layer::bullet};

static constexpr sf::Uint8 layer_num = 4;

static const sf::Vector2f left(-1.0, 0.0);
static const sf::Vector2f right(1.0, 0.0);
static const sf::Vector2f up(0.0, -1.0);
static const sf::Vector2f down(0.0, 1.0);
