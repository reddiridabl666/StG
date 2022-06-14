#pragma once

#include "Picture.hpp"
#include "Window.h"

class Background : public Picture {
public:
    Background() : Picture() {}
    Background(const sf::Texture& texture, const Window& window, Layer layer = Layer::Bg) :
        Picture(texture, window.getCenter(), layer) {
        auto factor = std::max(window.getView().getSize().x / texture.getSize().x,
                               window.getView().getSize().y / texture.getSize().y);
        scale(factor, factor);
    }
};
