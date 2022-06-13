#pragma once

// #include "SpriteObject.h"
#include "Picture.hpp"

template <typename T>
class HealthBar : public Picture {
protected:
    Picture outer_;
    const T& value_;
    float max_ = 0;
public:
    HealthBar(const sf::Texture& inner, const sf::Texture& outer, const T& value, sf::Vector2f pos = {}) :
        Picture(inner, pos),
        outer_(outer, pos),
        value_(value),
        max_(value) {}

    void setMax(const T& value) {
        max_ = value;
    }

    void update() override {
        if (max_ == 0) {
            return;
        }
        static const auto size = static_cast<sf::Vector2i>(getTexture()->getSize());
        setTextureRect({0, 0, static_cast<int>(value_ > 0 ? size.x * (value_ / max_) : 0), size.y});
    }
};
