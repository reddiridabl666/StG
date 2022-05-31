#pragma once

#include "GameObject.h"
#include <set>
#include <variant>

class CircleHitbox;
class RectHitbox;
class DynamicObject;

using HitboxInfo = std::variant<sf::Vector2f, float>;

inline const sf::Color green = {5, 240, 75};
inline const sf::Color transp_green = {5, 240, 75, 170};
inline const sf::Color red = {191, 34, 51};
inline const sf::Color transp_red = {191, 34, 51, 170};

class Hitbox : public GameObject {
protected:
    sf::Uint32 collision_num_ = 0;

    virtual bool collides_with_rect(const RectHitbox* other) const = 0;
    virtual bool collides_with_circle(const CircleHitbox* other) const = 0;
  
public:
    explicit Hitbox(Layer layer = Layer::Hitbox, sf::Vector2f size = {0, 0});
    // Hitbox(const Hitbox& hitbox);
    
    static Hitbox* getHitbox(const HitboxInfo& info, sf::Vector2f pos = {}, 
                             const sf::Color& fill = transp_green, const sf::Color& outline = green);

    static Hitbox* getFrameHitbox(const HitboxInfo& info, sf::Vector2f pos = {}, 
                                  const sf::Color& outline = green);

    virtual bool contains_point(const sf::Vector2f& point) const = 0;
    virtual bool collides_with(const Hitbox* other);

    virtual void on_collide();
    virtual void on_collide_stop();

    void check_collisions_with(Hitbox* other) {
        if (!other) {
            return;
        }
        if (collides_with(other)) {
            on_collide();
            other->on_collide();
        }
    }

    virtual void setFillColor(sf::Color color) = 0;
    virtual void setOutlineColor(sf::Color color) = 0;

    virtual void scale(float a, float b) {
        size_.x *= a;
        size_.y *= b;
    }

    sf::Uint32 getCollisionNum() {
        return collision_num_;
    }
    
    virtual HitboxInfo getInfo() const = 0;
     
    virtual ~Hitbox() = default;

    friend DynamicObject;
};

class RectHitbox : public Hitbox, public sf::RectangleShape {
 protected:
    bool collides_with_rect(const RectHitbox* other) const override;
    bool collides_with_circle(const CircleHitbox* other) const override;

public:
    explicit RectHitbox(const sf::Vector2f &size = {0, 0}, 
      const sf::Vector2f &center = {0, 0}, const sf::Color& fill = transp_green, 
      const sf::Color& outline = green, Layer layer = Layer::Hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    sf::Drawable* getDrawable() override {
        return this;
    }

    sf::Transformable* getTransformable() override {
        return this;
    }

    HitboxInfo getInfo() const override;

    void setFillColor(sf::Color color) override {
        sf::RectangleShape::setFillColor(color);
    }

    void setOutlineColor(sf::Color color) override {
        sf::RectangleShape::setOutlineColor(color);
    }

    sf::Vector2f getSize() const {
        return GameObject::getSize();
    }

    void scale(float a, float b) override {
        sf::RectangleShape::scale(a, b);
        Hitbox::scale(a, b);
        setOutlineThickness(-5 / max(a, b));
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
      const sf::Color& fill = transp_green, const sf::Color& outline = green,
      Layer layer = Layer::Hitbox);
      
    bool contains_point(const sf::Vector2f& point) const override;

    sf::Drawable* getDrawable() override {
        return this;
    }

    sf::Transformable* getTransformable() override {
        return this;
    }
    
    // sf::Vector2f getSize() const override;
    // sf::Vector2f getHalfSize() const override;

    HitboxInfo getInfo() const override;

    void setFillColor(sf::Color color) override {
        sf::CircleShape::setFillColor(color);
    }

    void setOutlineColor(sf::Color color) override {
        sf::CircleShape::setOutlineColor(color);
    }

    void scale(float a, float b) override {
        sf::CircleShape::scale(a, b);
        Hitbox::scale(a, b);
        setOutlineThickness(-5 / max(a, b));
    }

    float getRadius() const {
        return max(getHalfSize());
    }

    friend RectHitbox;
};
