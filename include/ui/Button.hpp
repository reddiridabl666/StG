#pragma once

#include "Text.hpp"
#include <functional>

class Button : public Text {
protected:
    std::function<void()> action_ = [] {};
    const sf::RenderWindow& window_;

    bool mouse_is_in_bounds() {
        auto pos = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        return getGlobalBounds().contains(pos);
    }
public:
    Button(const sf::String& text, const sf::Font& font, int size, const sf::RenderWindow& window, sf::Vector2f pos = {}, const std::function<void()>& action = [] {}) :
        Text(text, font, size, pos), action_(action), window_(window) {}

    Button(const sf::String& text, const sf::RenderWindow& window, sf::Vector2f pos = {}, const std::function<void()>& action = [] {}) :
        Button(text, DefaultFont, 48, window, pos, action) {}

    void setAction(const std::function<void()>& action) {
        action_ = action;
    }

    void update(float) override {
        if (!mouse_is_in_bounds()) {
            setFillColor(sf::Color::White);
            return;
        }
        setFillColor({200, 200, 200});
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            action_();
        }
    }
};
