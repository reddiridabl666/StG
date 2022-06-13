#pragma once

#include "Ui.hpp"

class Picture : public Ui, public sf::Sprite {
public:
    Picture(const sf::Texture& texture, sf::Vector2f pos, Layer layer = Layer::Ui) :
        Ui(layer),
        sf::Sprite(texture) {
        setOrigin(static_cast<sf::Vector2f>(texture.getSize() / 2u));
        setPosition(pos);
    }

    sf::Drawable* getDrawable() override {
        return this;
    }

    sf::Transformable* getTransformable() override {
        return this;
    }
};
