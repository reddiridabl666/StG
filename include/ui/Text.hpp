#pragma once

#include "Ui.hpp"
#include "Utils.hpp"
#include "Resources.hpp"

class Text : public Ui, public sf::Text {
public:
    static inline const sf::Font& DefaultFont = Resources::fonts["Nova+"];
    
    Text() : Ui(), sf::Text() {}

    Text(const sf::String& text, const sf::Font& font = DefaultFont, 
         int size = 48, sf::Vector2f pos = {0, 0}, Layer layer = Layer::Ui) :
            Ui(layer), sf::Text(text, font, size) {
            setOutlineThickness(5);
            setOutlineColor(sf::Color::Black);
            setPosition(pos);
    }

    Text(const sf::String& text, sf::Vector2f pos, int size = 48, Layer layer = Layer::Ui) : 
        Text(text, DefaultFont, size, pos, layer) {}

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

class CenteredText : public Text {
public:
    CenteredText() : Text() {
        setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
    }

     CenteredText(const sf::String& text, sf::Vector2f pos = {0, 0}, 
        int size = 48, Layer layer = Layer::Ui, const sf::Font& font = DefaultFont) :
        Text(text, font, size, pos, layer) {
        setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
    }
};

template <typename T>
class Log : public Text {
protected:
    sf::String text_;
    // int size_ = 48;
    const T* num_;
    bool centered = false;

public:
    Log() : Text(), text_(), num_() {}

    Log(const sf::String& text, const T& num, sf::Vector2f pos = {0, 0}, 
        int size = 48, Layer layer = Layer::Ui, bool centered = false, const sf::Font& font = DefaultFont) :
        Text(text + to_string(num), font, size, pos, layer),
        text_(text),
        // size_(size),
        num_(&num),
        centered(centered) {
        if (centered) {
            setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
        }
    }

    void update() override {
        if (num_)
            setString(text_ + to_string(*num_));
        else
            setString("Error");
    }

    void setText(const sf::String& text) {
        text_ = text;
    }

    void setString(const sf::String& string/* , int size */) {
        Text::setString(string);
        // setCharacterSize(size);
        if (centered) {
            setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
        }
    }

    // void setString(const sf::String& string) {
    //     setString(string, size_);
    // }
};
