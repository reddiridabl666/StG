#include <GameObject.h>

std::unordered_map<Layer, GameObject::objects> GameObject::objects_by_layer(layer_num);

GameObject::GameObject(Layer layer) : layer_(layer) {
    objects_by_layer[layer_].insert(this);
}

void GameObject::activate() {
    is_active_ = true;
}

void GameObject::deactivate() {
    is_active_ = false;
}

bool GameObject::is_active() const {
    return is_active_;
}

void GameObject::change_layer(Layer layer) {
    objects_by_layer[layer_].erase(this);
    layer_ = layer;
    objects_by_layer[layer_].insert(this);
}

void GameObject::draw_all(sf::RenderWindow& window) {
    for (const auto& layer : Layers) {
        for (auto obj : objects_by_layer[layer]) {
            if (obj->is_active()) {
                window.draw(*obj->getDrawable());
            }
        }
    }
}

GameObject::~GameObject() {
    objects_by_layer[layer_].erase(this);
}
