#pragma once

#include "GameObject.h"
#include "Resources.hpp"

class Text : public GameObject, public sf::Text {
public:
    Text() : GameObject(Layer::Interface), sf::Text() {}

    Text(const sf::String& text, const sf::Font& font = Resources::fonts["ARIAL"], 
         int size = 48, sf::Vector2f pos = {0, 0},
         Layer layer = Layer::Interface) :
            GameObject(layer), sf::Text(text, font, size) {
            setOrigin(getHalfSize());
            setPosition(pos);
            size_ = {getGlobalBounds().width, getGlobalBounds().height};
    }

    Text(const sf::String& text, sf::Vector2f pos) : 
        Text(text, Resources::fonts["ARIAL"], 48, pos) {}

    sf::Drawable* getDrawable() override {
        return this;
    }

    sf::Transformable* getTransformable() override {
        return this;
    }

    void setString(const sf::String& string) {
        sf::Text::setString(string);
        setOrigin(getHalfSize());
    }

    void setString(float num) {
        setString(std::to_string(num));
    }
};
