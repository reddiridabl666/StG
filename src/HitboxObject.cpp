#include "HitboxObject.h"

static const sf::Color green = {5, 240, 75};
static const sf::Color transp_green = {5, 240, 75, 200};
static const sf::Color red = {191, 34, 51};
static const sf::Color transp_red = {191, 34, 51, 200};

RectHitbox::RectHitbox(const sf::Vector2f &size, const sf::Vector2f &center, Layer layer) 
  : GameObjectBase(layer), sf::RectangleShape(size), height(size.y), width(size.x) {
    setOrigin(size.x / 2.f, size.y / 2.f);
    setPosition(center);
    setOutlineThickness(-4);
    setOutlineColor(green);
    setFillColor(transp_green);
}

// bool RectHitbox::contains_point(const sf::Vector2f& point) {
//     return (point.y <= top + height) && (point.y >= top) && 
//       (point.x <= left + width) && (point.x >= left);
// }

bool RectHitbox::collides_with(const RectHitbox& other) {
    if ((top >= other.top && top <= other.top + other.height) ||
        (other.top >= top && other.top <= top + height))
        if ((left >= other.left && left <= other.left + other.width) ||
            (other.left >= left && other.left <= left + width)) {
                return true;
        }
    return false;
}

void RectHitbox::setPosition(const sf::Vector2f& center) {
    sf::RectangleShape::setPosition(center);
    top = center.y - height / 2;
    left = center.x - width / 2;
}

void RectHitbox::setPosition(float x, float y) {
    setPosition({x, y});
}

void RectHitbox::move(const sf::Vector2f& offset) {
    setPosition(getPosition() + offset);
}

void RectHitbox::move(float x, float y) {
    move({x, y});
}

void RectHitbox::on_collide() {
    setOutlineColor(red);
    setFillColor(transp_red);
}

void RectHitbox::on_collide_stop() {
    setOutlineColor(green);
    setFillColor(transp_green);
}

void RectHitbox::scale(float factor) {
    sf::RectangleShape::scale(factor, factor);
}

const sf::Drawable* RectHitbox::get_drawable() const {
    return this;
}

HitboxObject::HitboxObject() : GameObject() {}

HitboxObject::HitboxObject(const sf::Texture& texture, sf::Vector2f hitbox_size, Layer layer) 
  : GameObject(texture, layer), hitbox_(hitbox_size) {}

void HitboxObject::setPosition(const sf::Vector2f& offset) {
    GameObject::setPosition(offset);
    hitbox_.setPosition(offset);
}

void HitboxObject::setPosition(float x, float y) {
    setPosition({x, y});
}

void HitboxObject::move(const sf::Vector2f& offset) {
    GameObject::move(offset);
    hitbox_.move(offset);
}

void HitboxObject::move(float x, float y) {
    move({x, y});
}

bool HitboxObject::collides_with(const Hitbox& hitbox) {
    return hitbox_.collides_with(hitbox);
}

bool HitboxObject::collides_with(const HitboxObject& obj) {
    return hitbox_.collides_with(obj.hitbox_);
}
