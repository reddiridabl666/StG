#pragma once

#include "Ui.hpp"
#include "Text.hpp"

class Picture : public Ui, public sf::Sprite {
public:
    Picture() : Ui(), sf::Sprite() {}
    
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

// class BackgroundedText : public CenteredText {
// protected:
//     Picture bg_;
// public:
//     BackgroundedText(const sf::String& text, const sf::Texture& bg, sf::Vector2f pos = {0, 0}, int size = 48, 
//                     Layer layer = Layer::Ui, const sf::Font& font = DefaultFont) :
//         CenteredText(text, pos + sf::Vector2f(0, -15), size, layer, font),
//         bg_(bg, pos, prev(layer)) {}
// };
