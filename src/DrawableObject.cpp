#include "DrawableObject.hpp"

static constexpr sf::Uint8 layer_num = 5;
const std::array<Layer, layer_num> Layers = {Layer::Bg, Layer::Character, Layer::Bullet, Layer::Hitbox, Layer::Ui};

std::unordered_map<Layer, DrawableObject::objects> DrawableObject::objects_by_layer;

void DrawableObject::draw_all(sf::RenderWindow& window) {
    for (const auto& layer : Layers) {
        for (auto obj : objects_by_layer[layer]) {
            if (!obj->is_hidden()) {
                window.draw(*obj->getDrawable());
            }
        }
    }
}
