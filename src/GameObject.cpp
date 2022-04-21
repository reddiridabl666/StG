#include <GameObject.h>

// ЛАЖА, ОБЪЕКТЫ ДОБАВЛЯЮТСЯ ПО НЕСКОЛЬКО РАЗ!!!

static void center(sf::Sprite &sprite, const sf::Texture &texture) {
    sprite.setOrigin(static_cast<float>(texture.getSize().x) / 2.f,
                     static_cast<float>(texture.getSize().y) / 2.f);
}

GameObject::GameObject() : texture_(nullptr), sprite_(), size_(), layer_() {
    all_objects.insert(this);
}

GameObject::GameObject(sf::Texture* texture, uint8_t layer) : GameObject() {
    layer_ = layer;
    setTexture(texture);
    objects_by_layer[layer_].insert(this);
}

void GameObject::setTexture(sf::Texture *texture) {
    texture_ = texture;
    sprite_.setTexture(*texture_);

    size_.x = static_cast<float>(texture_->getSize().x);
    size_.y = static_cast<float>(texture_->getSize().y);

    center(sprite_, *texture_);
}
// GameObject::GameObject(const std::string &image_path, unsigned int layer) : GameObject() {
//     loadFromFile(image_path);
//     layer_ = layer;
//     objects_by_layer[layer_].insert(this);
// }

// void GameObject::loadFromFile(const std::string &image_path) {
//     if (!texture_->loadFromFile(image_path)) {
//         std::cerr << "ERROR WHILE UPLOADING IMAGE FROM " << image_path << std::endl;
//         return;
//     }
//     sprite_.setTexture(*texture_);
//     size_.x = static_cast<float>(texture_->getSize().x);
//     size_.y = static_cast<float>(texture_->getSize().y);
//     center(sprite_, *texture_);
// }

const sf::Sprite &GameObject::getSprite() const {
    return sprite_;
}

void GameObject::setPosition(float x, float y) {
    sprite_.setPosition(x, y);
}

void GameObject::move(float x, float y) {
    sprite_.move(x, y);
}

void GameObject::move(sf::Vector2f offset) {
    sprite_.move(offset);
}

void GameObject::scale(float factor) {
    sprite_.scale(factor, factor);
    size_ *= factor;
}

sf::Vector2f GameObject::getSize() const {
    return size_;
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

GameObject::~GameObject() {
    all_objects.erase(this);
    objects_by_layer[layer_].erase(this);
}

void GameObject::change_layer(uint8_t layer) {
    objects_by_layer[layer_].erase(this);
    layer_ = layer;
    objects_by_layer[layer_].insert(this);
}

void GameObject::draw_all(sf::RenderWindow& window) {
    for (const auto& layer : GameObject::objects_by_layer) {
        for (auto obj : layer) {
            if (obj->is_active()) {
                window.draw(obj->getSprite());
            }
        }
    }
}

void GameObject::scale_all(float factor) {
    for (auto obj : all_objects) {
        obj->scale(factor);
    }
}
