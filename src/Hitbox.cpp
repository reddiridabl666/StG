#include "Hitbox.hpp"
#include <set>

Hitbox* Hitbox::getHitbox(const HitboxInfo& info, sf::Vector2f pos, const sf::Color& fill, const sf::Color& outline) {
    if (auto size = std::get_if<sf::Vector2f>(&info)) {
        return new RectHitbox(*size, pos, fill, outline);
    } else if (auto radius = std::get_if<float>(&info)) {
        return new CircleHitbox(*radius, pos, fill, outline);
    }
    return nullptr;
}

Hitbox* Hitbox::getFrameHitbox(const HitboxInfo& info, sf::Vector2f pos, const sf::Color& outline) {
    return getHitbox(info, pos, sf::Color::Transparent, outline); 
}

Hitbox::Hitbox(Layer layer, sf::Vector2f size) : GameObject(layer, size) {
#ifndef DEBUG
    hide();
#endif
}

// Hitbox::Hitbox(const Hitbox& hitbox) : GameObject(hitbox.layer_) {}

bool Hitbox::collides_with(const Hitbox* other) {
    if (auto target = dynamic_cast<const RectHitbox*>(other)) {return collides_with_rect(target);}
    if (auto target = dynamic_cast<const CircleHitbox*>(other)) {return collides_with_circle(target);}
    return false;
}

void Hitbox::on_collide() {
    collision_num_++;
    setOutlineColor(red);
    setFillColor(transp_red);
}

void Hitbox::on_collide_stop() {
    setOutlineColor(green);
    setFillColor(transp_green);
}

RectHitbox::RectHitbox(const sf::Vector2f &size, const sf::Vector2f &center, 
                       const sf::Color& fill, const sf::Color& outline, Layer layer) 
  : Hitbox(layer, size), sf::RectangleShape(size) {
    setOrigin(size.x / 2.f, size.y / 2.f);
    setPosition(center);
    setOutlineThickness(-5);
    setFillColor(fill);
    setOutlineColor(outline);
}

bool RectHitbox::contains_point(const sf::Vector2f& point) const {
    return getGlobalBounds().contains(point);
}

HitboxInfo RectHitbox::getInfo() const {
    return getSize();
}

bool RectHitbox::collides_with_rect(const RectHitbox* other) const {
    return getGlobalBounds().intersects(other->getGlobalBounds());
}

bool RectHitbox::collides_with_circle(const CircleHitbox* other) const {
    return other->collides_with_rect(this);
}

CircleHitbox::CircleHitbox(float radius, const sf::Vector2f &center,
                           const sf::Color& fill, const sf::Color& outline, Layer layer) 
  : Hitbox(layer, {radius * 2, radius * 2}), sf::CircleShape(radius) {
    setOrigin(radius, radius);
    // size_ = sf::Vector2f{radius, radius} * 2.f ;
    setPosition(center);
    setOutlineThickness(-5);
    setFillColor(fill);
    setOutlineColor(outline);
}
      
bool CircleHitbox::contains_point(const sf::Vector2f& point) const {
    return squared_distance(getPosition(), point) <= getRadius() * getRadius();
}

static bool is_beetween(float a,float b, float c) {
    return b >= a && b <= c;
}

bool CircleHitbox::collides_with_rect(const RectHitbox* other) const {
    auto rect = other->getGlobalBounds();

    if (other->contains_point(getPosition())) {
        return true;
    }

    if (contains_point({rect.left, rect.top}) || 
        contains_point({rect.left, rect.top + rect.height}) ||
        contains_point({rect.left + rect.width, rect.top}) || 
        contains_point({rect.left + rect.width, rect.top + rect.height})) {
            return true;
    }

    if (((std::abs(rect.top - getPosition().y) <= getRadius() || std::abs(rect.top + rect.height - getPosition().y) <= getRadius()) 
            && is_beetween(0, getPosition().x - rect.left, rect.width)) ||
        ((std::abs(rect.left - getPosition().x) <= getRadius() || std::abs(rect.left + rect.width - getPosition().x) <= getRadius())
            && is_beetween(0, getPosition().y - rect.top, rect.height))) {
            return true;
    }
    return false;
}

bool CircleHitbox::collides_with_circle(const CircleHitbox* other) const {
    return squared_distance(getPosition(), other->getPosition()) <= pow(getRadius() + other->getRadius(), 2);
}

// sf::Vector2f CircleHitbox::getSize() const {
//     return getHalfSize() * 2.f;
// }

// sf::Vector2f CircleHitbox::getHalfSize() const {
//     return {getRadius(), getRadius()};
// }

HitboxInfo CircleHitbox::getInfo() const {
    return getHalfSize().x;
}
