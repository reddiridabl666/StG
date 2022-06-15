#pragma once

#include "Picture.hpp"
#include "Button.hpp"

class Menu : public Picture {
protected:
    std::vector<Button> buttons_;
    int hover_ptr = -1;

    sf::Vector2f start_pos() {
        return getPosition() + sf::Vector2f{0, 80 - static_cast<float>(getTexture()->getSize().y / 2)};
    }

    Window& window_;
    float delta = 100;
public:
    Menu(const sf::Texture& texture, sf::Vector2f pos, Window& window, const std::vector<Button::Info>& infos, Layer layer = Layer::Menu2) :
        Picture(texture, pos, layer), window_(window) {
        window_.reset_menu(this);
        buttons_.resize(infos.size());

        size_t i = 0;
        for (auto& info : infos) {
            buttons_[i] = Button(info.text, window, sf::Vector2f(0, delta * i) + start_pos() + info.pos, 
                                 72, info.action, next(layer));
            buttons_[i].in_menu = true;
            ++i;
        }
    }
    
    ssize_t buttonNum() const {
        return buttons_.size();
    }

    Button& addButton(const Button::Info& info, sf::Vector2f pos = {}, int size = 72) {
        const sf::Vector2f pos_ = [&] {
            // if (info.pos != sf::Vector2f()) {
            //     return info.pos;
            // }
            if (pos != sf::Vector2f()) {
                return pos;
            }
            if (buttons_.empty()) {
                return start_pos();
            } else {
                return start_pos() + sf::Vector2f(0, delta) * static_cast<float>((buttons_.size()));
            }
        }();
        
        buttons_.emplace_back(info.text, window_, pos_ + info.pos, size, info.action, next(layer_));
        buttons_.back().in_menu = true;
        return buttons_.back();
    }

    const std::vector<Button>& getButtons() const {
        return buttons_;
    }
    
    void update() override {
        Picture::update();
        auto it = std::find_if(buttons_.begin(),
                               buttons_.end(),
                               [] (Button& it) {return it.mouse_is_in_bounds();});
        if (it == buttons_.end()) {
            hover_ptr = -1;
            return;
        }
        hover_ptr = std::distance(buttons_.begin(), it);
    }

    virtual ~Menu() {
        window_.reset_menu(nullptr);
    }

    friend Window;
};

class LabeledMenu : public Menu {
protected:
    Text label_;
public:
    LabeledMenu(const sf::String& text, const sf::Texture& texture, sf::Vector2f pos, Window& window, 
              const std::vector<Button::Info>& infos, Layer layer = Layer::Menu2) :
        Menu(texture, pos, window, {}, layer), 
        label_(text, Text::DefaultFont, 90, pos + sf::Vector2f{0, 70 - static_cast<float>(texture.getSize().y / 2)}) {
        label_.setOrigin(sf::Vector2f{label_.getLocalBounds().width, label_.getLocalBounds().height} / 2.f);

        if (infos.empty()) {
            return;
        }

        addButton(infos.front(), start_pos() + sf::Vector2f(0, delta));

        for (auto it = ++infos.begin(); it != infos.end(); ++it) {
            addButton(*it, start_pos() + sf::Vector2f(0, delta * (std::distance(infos.begin(), it) + 1)));
        }
    }

    friend Window;
};
