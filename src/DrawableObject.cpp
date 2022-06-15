#include "DrawableObject.hpp"
#include <algorithm>

static constexpr sf::Uint8 layer_num = 7;
static constexpr std::array<Layer, layer_num> Layers = {Layer::Bg, Layer::Character, Layer::Bullet, Layer::Hitbox, Layer::Menu, Layer::Menu2, Layer::Ui};

Layer prev(Layer layer) {
    auto it = std::distance(Layers.begin(), std::find(Layers.begin(), Layers.end(), layer));
    return it > 0 ? Layers[it - 1] : Layer::Bg;
}

Layer next(Layer layer) {
    auto it = std::distance(Layers.begin(), std::find(Layers.begin(), Layers.end(), layer));
    return it < layer_num - 1 ? Layers[it + 1] : Layer::Ui;
}

std::unordered_map<Layer, DrawableObject::objects> DrawableObject::objects_by_layer;

void DrawableObject::draw_all(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    for (const auto& layer : Layers) {
        for (auto obj : objects_by_layer[layer]) {
            if (!obj->is_hidden()) {
                window.draw(*obj->getDrawable());
            }
        }
    }
    window.display();
}
