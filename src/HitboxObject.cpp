#include "HitboxObject.h"

RectHitbox::RectHitbox(const sf::Vector2f &size, const sf::Vector2f &center) 
  : sf::RectangleShape(size), top((center - size / 2.f).y), 
    left((center - size / 2.f).x), height(size.y), width(size.x) {
    setOrigin(size.x / 2.f, size.y / 2.f);
    setPosition(center);
    setOutlineThickness(5);
    setOutlineColor({5, 240, 75});
    setFillColor({5, 240, 75, 200});
}

bool RectHitbox::contains_point(const sf::Vector2f& point) {
    return (point.y <= top + height) && (point.y >= top) && 
      (point.x <= left + width) && (point.x >= left);
}

bool RectHitbox::collides_with(const RectHitbox& target) {
    if ((top <= target.top && top >= target.top + target.height) ||
        (target.top <= top && target.top >= top + height))
        if ((left <= target.left + target.width && left >= target.left) ||
            (target.left <= left + width && target.left >= left))
                return true;
    return false;
}

HitboxObject::HitboxObject() : GameObject() {}

HitboxObject::HitboxObject(const sf::Texture& texture, sf::Vector2f hitbox_size, sf::Uint8 layer) 
  : GameObject(texture, layer), hitbox_(hitbox_size) {

}
