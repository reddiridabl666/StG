#include "SpriteObject.h"

SpriteObject::SpriteObject(Layer layer) : GameObject(layer), sf::Sprite(), size_() {}

SpriteObject::SpriteObject(const sf::Texture& texture, sf::Vector2f pos, Layer layer) : SpriteObject(layer) {
    setTexture(texture);
    setPosition(pos);
}

SpriteObject::SpriteObject(const SpriteObject& other) : SpriteObject(*other.getTexture(), other.getPosition(), other.layer_) {}

static void center(sf::Sprite &sprite, const sf::Texture &texture) {
    sprite.setOrigin(static_cast<float>(texture.getSize().x) / 2.f,
                     static_cast<float>(texture.getSize().y) / 2.f);
}

void SpriteObject::setTexture(const sf::Texture& texture) {
    sf::Sprite::setTexture(texture);
    size_ = static_cast<sf::Vector2f>(texture.getSize());
    center(*this, texture);
}

sf::Drawable* SpriteObject::getDrawable() {
    return this;
}

sf::Transformable* SpriteObject::getTransformable() {
    return this;
}

sf::Vector2f SpriteObject::getSize() const {
    return size_;
}

sf::Vector2f SpriteObject::getHalfSize() const {
    return size_ / 2.f;
}
