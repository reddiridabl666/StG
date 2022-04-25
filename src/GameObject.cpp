#include <GameObject.h>

std::unordered_map<Layer, GameObjectBase::objects> GameObjectBase::objects_by_layer(layer_num);

GameObjectBase::GameObjectBase(Layer layer) : layer_(layer) {
    objects_by_layer[layer_].insert(this);
}

void GameObjectBase::activate() {
    is_active_ = true;
}

void GameObjectBase::deactivate() {
    is_active_ = false;
}

bool GameObjectBase::is_active() const {
    return is_active_;
}

void GameObjectBase::change_layer(Layer layer) {
    objects_by_layer[layer_].erase(this);
    layer_ = layer;
    objects_by_layer[layer_].insert(this);
}

void GameObjectBase::draw_all(sf::RenderWindow& window) {
    for (const auto& layer : Layers) {
        for (auto obj : objects_by_layer[layer]) {
            if (obj->is_active()) {
                window.draw(*obj->get_drawable());
            }
        }
    }
}

GameObjectBase::~GameObjectBase() {
    objects_by_layer[layer_].erase(this);
}

GameObject::GameObject(Layer layer) : GameObjectBase(layer), sf::Sprite(), size_() {}

GameObject::GameObject(const sf::Texture& texture, Layer layer) : GameObject(layer) {
    setTexture(texture);
}

static void center(sf::Sprite &sprite, const sf::Texture &texture) {
    sprite.setOrigin(static_cast<float>(texture.getSize().x) / 2.f,
                     static_cast<float>(texture.getSize().y) / 2.f);
}

void GameObject::setTexture(const sf::Texture& texture) {
    sf::Sprite::setTexture(texture);
    size_ = static_cast<sf::Vector2f>(texture.getSize());
    center(*this, texture);
}

sf::Drawable* GameObject::get_drawable() {
    return this;
}

sf::Vector2f GameObject::getSize() const {
    return size_;
}
