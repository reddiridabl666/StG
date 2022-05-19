#pragma once

#include "GameObject.h"
#include <set>

class CircleHitbox;
class RectHitbox;
class DynamicObject;

class Hitbox : public GameObject {
protected:
    sf::Uint32 collision_num_ = 0;

    virtual bool collides_with_rect(const RectHitbox* other) const = 0;
    virtual bool collides_with_circle(const CircleHitbox* other) const = 0;
  
public:
    explicit Hitbox(Layer layer = Layer::Hitbox);
    Hitbox(const Hitbox& hitbox);

    virtual bool contains_point(const sf::Vector2f& point) const = 0;
    virtual bool collides_with(const Hitbox* other);

    virtual void on_collide();
    virtual void on_collide_stop();
    virtual sf::Vector2f getSize() const = 0;
    virtual void setFillColor(sf::Color color) = 0;
     
    virtual ~Hitbox() = default;

    friend DynamicObject;
};

class RectHitbox : public Hitbox, public sf::RectangleShape {
 protected:
    bool collides_with_rect(const RectHitbox* other) const override;
    bool collides_with_circle(const CircleHitbox* other) const override;

public:
    explicit RectHitbox(const sf::Vector2f &size = {0, 0}, 
      const sf::Vector2f &center = {0, 0}, 
      Layer layer = Layer::Hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    sf::Drawable* getDrawable() override;
    sf::Transformable* getTransformable() override;

    sf::Vector2f getSize() const override;
    sf::Vector2f getHalfSize() const override;

    void setFillColor(sf::Color color) override {
        sf::RectangleShape::setFillColor(color);
    }

    friend CircleHitbox;
};

class CircleHitbox : public Hitbox, public sf::CircleShape {
protected:
    bool collides_with_rect(const RectHitbox* other) const override;
    bool collides_with_circle(const CircleHitbox* other) const override;

public:
    explicit CircleHitbox(float radius = 0, 
      const sf::Vector2f &center = {0, 0}, 
      Layer layer = Layer::Hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    sf::Drawable* getDrawable() override;
    sf::Transformable* getTransformable() override;
    
    sf::Vector2f getSize() const override;
    sf::Vector2f getHalfSize() const override;

    void setFillColor(sf::Color color) override {
        sf::CircleShape::setFillColor(color);
    }

    friend RectHitbox;
};
