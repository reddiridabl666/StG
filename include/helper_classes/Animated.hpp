#pragma once

#include "SFML/Graphics.hpp"

class Animated {
public:
    using Sprites = std::unordered_map<std::string, std::vector<sf::Texture>>;
protected:
    Sprites sprites_;
    sf::Clock animation_clock_;
    
    sf::Uint8 fps_ = 10;
    const std::vector<sf::Texture>* frames_ = nullptr;
    size_t cur_ = 0;

    // virtual void init_sprites(sf::Image sprite_sheet) = 0;

    void setFPS(sf::Uint8 fps) {
        fps_ = fps;
    }
    
    void setAnimation(const std::vector<sf::Texture>& frames) {
        frames_ = &frames;
        if (cur_ >= frames.size()) {
            cur_ = 0;
        }
        setTexture(frames[cur_]);   
    }

    void update() {
        if (!frames_ || frames_->empty()) {
            return;
        }
        if (animation_clock_.getElapsedTime().asSeconds() >= 1.f / fps_) {
            animation_clock_.restart();
            if (cur_ >= frames_->size() - 1) {
                cur_ = -1;
            }
            setTexture((*frames_)[++cur_]);
        }
    }

public:
    Animated(std::unordered_map<std::string, std::vector<sf::Texture>> sprites = {}) : sprites_(sprites) {}

    virtual void setTexture(const sf::Texture&) = 0;
};
