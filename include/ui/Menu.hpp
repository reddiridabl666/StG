#pragma once

#include "Picture.hpp"
#include "Button.hpp"

class Menu : public Picture {
protected:
    std::vector<Button> buttons_;
    const sf::RenderWindow* window_;
    float delta = 100;
public:
    Menu(const sf::Texture& texture, sf::Vector2f pos, const sf::RenderWindow& window, const std::vector<Button::Info>& infos, Layer layer = Layer::Menu2) :
        Picture(texture, pos, layer), window_(&window) {
        buttons_.resize(infos.size());

        size_t i = 0;
        for (auto& info : infos) {
            buttons_[i] = Button(info.text, window, pos + sf::Vector2f(0, delta * i + 80 - static_cast<float>(texture.getSize().y / 2)), 
                                 72, info.action, next(layer));
            ++i;
        }
    }
    
    void addButton(const Button::Info& info, int size = 72) {
        const sf::Vector2f pos = [this] {
            if (buttons_.empty()) {
                return getPosition() + sf::Vector2f{0, 80 - static_cast<float>(getTexture()->getSize().y / 2)};
            } else {
                return buttons_.back().getPosition() + sf::Vector2f(0, delta);
            }
        }();
        
        buttons_.emplace_back(info.text, *window_, pos, size, info.action, next(layer_));
    }
};

class EndScreen : public Menu {
protected:
    Text label_;
public:
    EndScreen(const sf::String& text, const sf::Texture& texture, sf::Vector2f pos, const sf::RenderWindow& window, 
              const std::vector<Button::Info>& infos, Layer layer = Layer::Menu2) :
        Menu(texture, pos, window, {}, layer), label_(text, Text::DefaultFont, 90, 
                                                pos + sf::Vector2f{0, 70 - static_cast<float>(texture.getSize().y / 2)}) {
        label_.setOrigin(sf::Vector2f{label_.getLocalBounds().width, label_.getLocalBounds().height} / 2.f);
        addButton({});
        for (auto& info : infos) {
            addButton(info);
        }
    }
};
