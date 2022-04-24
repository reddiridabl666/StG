// #include "HitboxObject.h"

// template <typename HitboxType>
// HitboxObject<HitboxType>::HitboxObject(Layer layer) : GameObject(layer) {}

// template <typename HitboxType>
// HitboxObject<HitboxType>::HitboxObject(const sf::Texture& texture, sf::Vector2f hitbox_size, Layer layer) 
//   : GameObject(texture, layer), hitbox_(hitbox_size) {}

// template <typename HitboxType>
// void HitboxObject<HitboxType>::setPosition(const sf::Vector2f& offset) {
//     GameObject::setPosition(offset);
//     hitbox_.setPosition(offset);
// }

// template <typename HitboxType>
// void HitboxObject<HitboxType>::setPosition(float x, float y) {
//     setPosition({x, y});
// }

// template <typename HitboxType>
// void HitboxObject<HitboxType>::move(const sf::Vector2f& offset) {
//     GameObject::move(offset);
//     hitbox_.move(offset);
// }

// template <typename HitboxType>
// void HitboxObject<HitboxType>::move(float x, float y) {
//     move({x, y});
// }

// template <typename HitboxType>
// bool HitboxObject<HitboxType>::collides_with(const Hitbox* hitbox) {
//     return hitbox_.collides_with(hitbox);
// }

// template <typename HitboxType>
// bool HitboxObject<HitboxType>::collides_with(const HitboxObject& obj) {
//     return hitbox_.collides_with(&obj.hitbox_);
// }
