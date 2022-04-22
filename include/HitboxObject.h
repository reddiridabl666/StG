#pragma once

#include "GameObject.h"

// Пока сделаем только прямоугольные, но после хотелось бы придумать какое-нибудь 
// наследование для окружностей и произвольных выпуклых многоугольников

// class Hitbox {
//   virtual bool collides_with(const Hitbox& target) = 0;
// };

class RectHitbox : public sf::RectangleShape {
  protected:
    float top, left, height, width;
  public:
    RectHitbox(const sf::Vector2f &size = {0, 0}, const sf::Vector2f &center = {0, 0});
    // void setPosition(const sf::Vector2f &center);
    // void setSize(const sf::Vector2f &size);
    bool contains_point(const sf::Vector2f& point);
    bool collides_with(const RectHitbox& target);
};

using Hitbox = RectHitbox;

// template <typename HitboxType>
class HitboxObject : public GameObject {
  protected:
    Hitbox hitbox_;

  public:
    HitboxObject();
    HitboxObject(const sf::Texture& texture, sf::Vector2f hitbox_size, sf::Uint8 layer = 0);
};
