#pragma once

#include "Text.hpp"
#include <functional>

// class BackgroundedText : public Text {
// protected:
//     Picture bg_;
// public:
//     BackgroundedText(const sf::String& text, const sf::Texture& bg, sf::Vector2f pos = {0, 0}, int size = 48, 
//                     Layer layer = Layer::Ui, const sf::Font& font = DefaultFont) :
//         Text(text, font, size, pos, layer),
//         bg_(bg, pos, prev(layer)) {}
// };

class Button : public /* Backgrounded */Text {
protected:
    // static inline const sf::Texture& DefaultBackground = Resources::textures["button_normal"];
    bool flag = false;
    std::function<void()> action_ = [] {};
    const sf::RenderWindow* window_;

    bool mouse_is_in_bounds() {
        auto pos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
        return getGlobalBounds().contains(pos);
    }

public:
    struct Info {
        sf::String text = "";
        std::function<void()> action = [] {};
    };

    Button() = default;

    Button(const sf::String& text, const sf::RenderWindow& window, sf::Vector2f pos = {}, int size = 48, 
           const std::function<void()>& action = [] {}, Layer layer = Layer::Ui, const sf::Font& font = DefaultFont) :
        Text(text, font, size,/* DefaultBackground, */ pos, layer), action_(action), window_(&window) {
        setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
    }

    void setAction(const std::function<void()>& action) {
        action_ = action;
    }

    void update() override {
        if (!mouse_is_in_bounds()) {
            setFillColor(sf::Color::White);
            return;
        }
        setFillColor({200, 200, 200});
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            flag = true;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && flag) {
            action_();
        }
    }
};
