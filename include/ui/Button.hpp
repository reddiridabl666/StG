#pragma once

#include "Text.hpp"
#include "Window.h"
#include <functional>

class Button : public Text {
protected:
    bool pressable = false;
    bool hovered = true;
    bool in_menu = false;

    std::function<void()> action_ = [] {};
    Window* window_;

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

    Button(const sf::String& text, Window& window, sf::Vector2f pos = {}, int size = 48, 
           const std::function<void()>& action = [] {}, Layer layer = Layer::Ui, const sf::Font& font = DefaultFont) :
        Text(text, font, size,/* DefaultBackground, */ pos, layer), action_(action), window_(&window) {
        setOrigin(sf::Vector2f{getLocalBounds().width, getLocalBounds().height} / 2.f);
    }

    void activate() {
        if (pressable) {
            action_();
        }
    }

    void setAction(const std::function<void()>& action) {
        action_ = action;
    }

    void hover() {
        setFillColor({180, 180, 180});
        hovered = true;
    }

    void unhover() {
        setFillColor(sf::Color::White);
        hovered = false;
    }

    void update() override {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            pressable = true;
        }

        unhover();

        if (!in_menu && mouse_is_in_bounds()) {
            hover();
        }

        if (!hovered) {
            return;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            activate();
        }
    }

    friend Window;
    friend Menu;
};
