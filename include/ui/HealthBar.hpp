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
    HealthBar(const sf::Texture& inner, const sf::Texture& outer, const T& value, sf::Vector2f pos = {}, Layer layer = Layer::Ui) :
        Picture(inner, pos, layer),
        outer_(outer, pos, prev(layer)),
        value_(value),
        max_(value) {}

    void setMax(const T& value) {
        max_ = value;
    }

    void show() override {
        Picture::show();
        outer_.show();
    }

    void hide() override {
        Picture::hide();
        outer_.hide();
    }

    void change_layer(Layer layer) {
        Picture::change_layer(layer);
        outer_.change_layer(layer);
    }

    void update() override {
        if (max_ == 0) {
            return;
        }
        static const auto size = static_cast<sf::Vector2i>(getTexture()->getSize());
        setTextureRect({0, 0, static_cast<int>(value_ > 0 ? size.x * (value_ / max_) : 0), size.y});
    }
};
