#pragma once

#include "GameObject.h"

// Пока сделаем только прямоугольные, но после хотелось бы придумать какое-нибудь 
// наследование для окружностей и произвольных выпуклых многоугольников

class CircleHitbox;
class RectHitbox;

class Hitbox : public GameObjectBase {
  protected:
    virtual bool collides_with_rect(const RectHitbox* other) const = 0;
    virtual bool collides_with_circle(const CircleHitbox* other) const = 0;

  public:
    using GameObjectBase::GameObjectBase;
    virtual bool contains_point(const sf::Vector2f& point) const = 0;
    virtual bool collides_with(const Hitbox* other);

    virtual void on_collide();
    virtual void on_collide_stop();
};

class RectHitbox : public Hitbox, public sf::RectangleShape {
  protected:
    float top_, left_, height_, width_;
    bool collides_with_rect(const RectHitbox* other) const override;
    bool collides_with_circle(const CircleHitbox* other) const override;

  public:
    explicit RectHitbox(const sf::Vector2f &size = {0, 0}, 
      const sf::Vector2f &center = {0, 0}, 
      Layer layer = Layer::hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    void setPosition(const sf::Vector2f& center);
    void setPosition(float x, float y);
    void move(const sf::Vector2f& center);
    void move(float x, float y);

    // void on_collide() override;
    // virtual void on_collide_stop() override;

    void scale(float factor) override;
    sf::Drawable* get_drawable() override;

    friend CircleHitbox;
};

class CircleHitbox : public Hitbox, public sf::CircleShape {
  protected:
    bool collides_with_rect(const RectHitbox* other) const override;
    bool collides_with_circle(const CircleHitbox* other) const override;

  public:
    explicit CircleHitbox(float radius = 0, 
      const sf::Vector2f &center = {0, 0}, 
      Layer layer = Layer::hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    // void on_collide() override;
    // virtual void on_collide_stop() override;

    void scale(float factor) override;
    sf::Drawable* get_drawable() override;

    friend RectHitbox;
};
