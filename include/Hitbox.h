#pragma once

#include "GameObject.h"
#include <set>

class CircleHitbox;
class RectHitbox;
class DynamicObject;

class Hitbox : public GameObjectBase {
  protected:
    sf::Uint32 collision_num_ = 0;

    virtual bool collides_with_rect(const RectHitbox* other) const = 0;
    virtual bool collides_with_circle(const CircleHitbox* other) const = 0;
  
  private:
    // static void refresh_collision_num();
  
  public:
    // static std::set<Hitbox*> all;

    explicit Hitbox(Layer layer = Layer::Hitbox);
    Hitbox(const Hitbox& hitbox);
    // static void check_collisions();

    virtual bool contains_point(const sf::Vector2f& point) const = 0;
    virtual bool collides_with(const Hitbox* other);

    virtual void on_collide();
    virtual void on_collide_stop();
    virtual sf::Vector2f getSize() const = 0;
     
    virtual ~Hitbox();

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
    sf::Vector2f getSize() const override;

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
    sf::Vector2f getSize() const override;

    friend RectHitbox;
};
