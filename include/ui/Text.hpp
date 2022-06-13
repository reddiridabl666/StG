#pragma once

#include "Ui.hpp"
#include "Resources.hpp"

class Text : public Ui, public sf::Text {
protected:
    static inline const sf::Font& DefaultFont = Resources::fonts["Nova+"];
public:
    Text() : Ui(), sf::Text() {}

    Text(const sf::String& text, const sf::Font& font = DefaultFont, 
         int size = 48, sf::Vector2f pos = {0, 0}) :
            Ui(), sf::Text(text, font, size) {
            setOutlineThickness(2);
            setOutlineColor(sf::Color::Black);
            setPosition(pos);
    }

    Text(const sf::String& text, sf::Vector2f pos) : 
        Text(text, DefaultFont, 48, pos) {}

    sf::Drawable* getDrawable() override {
        return this;
    }

    sf::Transformable* getTransformable() override {
        return this;
    }

    void setString(const sf::String& string) {
        sf::Text::setString(string);
    }

    void setString(float num) {
        setString(std::to_string(num));
    }
};

template <typename T>
class Log : public Text {
protected:
    sf::String text_;
    const T* num_;

public:
    Log() : Text(), text_(), num_() {}

    Log(const sf::String& text, const T& num, sf::Vector2f pos = {0, 0}) :
        Text(text + std::to_string(num), pos), text_(text), num_(&num) {}

    void update() override {
        if (num_)
            setString(text_ + std::to_string(*num_));
        else
            setString("Error");
    }

    void setText(const sf::String& text) {
        text_ = text;
    }
};
