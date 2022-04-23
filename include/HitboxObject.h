#pragma once

#include "GameObject.h"

// Пока сделаем только прямоугольные, но после хотелось бы придумать какое-нибудь 
// наследование для окружностей и произвольных выпуклых многоугольников

class RectHitbox : public GameObjectBase, public sf::RectangleShape {
  protected:
    float top, left, height, width;
  public:
    RectHitbox(const sf::Vector2f &size = {0, 0}, 
      const sf::Vector2f &center = {0, 0}, 
      Layer layer = Layer::hitbox);
    bool contains_point(const sf::Vector2f& point);
    bool collides_with(const RectHitbox& target);

    void scale(float factor) override;
    const sf::Drawable* get_drawable() const override;
};

using Hitbox = RectHitbox;

class HitboxObject : public GameObject {
  protected:
    Hitbox hitbox_;

  public:
    HitboxObject();
    HitboxObject(const sf::Texture& texture, sf::Vector2f hitbox_size = {0, 0}, Layer layer = Layer::character);

    void setPosition(const sf::Vector2f& offset);
    void setPosition(float x, float y);

    void move(const sf::Vector2f& offset);
    void move(float x, float y);

    bool collides_with(const Hitbox& hitbox);
    bool collides_with(const HitboxObject& obj);
};
