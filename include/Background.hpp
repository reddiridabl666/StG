#pragma once

#include "GameObject.h"
#include "Window.h"

class Background : SpriteObject {
public:
    Background() : SpriteObject(Layer::Bg) {}
    Background(const sf::Texture& texture, const Window& window) :
        SpriteObject(texture, window.getCenter(), Layer::Bg) {
        auto factor = std::max(static_cast<float>(window.getSize().x) / getSize().x,
                               static_cast<float>(window.getSize().y) / getSize().y);
        scale(factor, factor);
    }
};
