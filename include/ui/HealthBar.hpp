#pragma once

#include "SpriteObject.h"

template <typename T>
class HealthBar : public SpriteObject {
protected:
    SpriteObject outer_;
    const T& value_;
    float max_ = 0;
public:
    HealthBar(const sf::Texture& inner, const sf::Texture& outer, const T& value, sf::Vector2f pos = {}) :
        SpriteObject(inner, pos, Layer::Ui),
        outer_(outer, pos, Layer::Ui),
        value_(value),
        max_(value) {}

    void setMax(const T& value) {
        max_ = value;
    }

    void update(float) override {
        if (max_ == 0) {
            return;
        }
        static const auto size = static_cast<sf::Vector2i>(getTexture()->getSize());
        setTextureRect({0, 0, /* size.x / 2 */ static_cast<int>(value_ > 0 ? size.x * (value_ / max_) : 0), size.y});
    }
};
