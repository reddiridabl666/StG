#pragma once

#include "GameObject.h"

class Text : public GameObject, public sf::Text {
public:
    Text(const sf::String& text, const sf::Font& font, 
         int size = 48, sf::Vector2f pos = {0, 0},
         Layer layer = Layer::Interface) :
            GameObject(layer), sf::Text(text, font, size) {
            setOrigin(getHalfSize());
            setPosition(pos);
        }

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

    sf::Vector2f getSize() const {
        return {getGlobalBounds().width, getGlobalBounds().height};
    }

    sf::Vector2f getHalfSize() const override {
        return getSize() / 2.f;
    }
};

// class AutoUpdatingText : Text {
// private:
//     const sf::String* string_ptr_;
// public:
//     AutoUpdatingText(const sf::String* text, const sf::Font& font, 
//                      int size = 48, sf::Vector2f pos = {0, 0},
//                      Layer layer = Layer::Interface) : 
//             Text(text ? *text : "", font, size, pos), 
//             string_ptr_(text) {}

//     void update() {
//         if (!string_ptr_)
//             return;
//         setString(*string_ptr_);
//     }
// };
