#pragma once

#include "GameObject.h"
#include "Hitbox.h"

template <typename HitboxType>
class HitboxObject : public GameObject {
  public:
  // protected:
    HitboxType hitbox_;

  public:
    HitboxObject(Layer layer = Layer::character) : GameObject(layer) {}
    HitboxObject(const sf::Texture& texture, const HitboxType& hitbox, Layer layer = Layer::character) 
        : GameObject(texture, layer), hitbox_(hitbox) {};

    void setPosition(const sf::Vector2f& offset) {
        GameObject::setPosition(offset);
        hitbox_.setPosition(offset);
    }

    void setPosition(float x, float y) {
        setPosition({x, y});
    }

    void move(const sf::Vector2f& offset) {
        GameObject::move(offset);
        hitbox_.move(offset);
    }

    void move(float x, float y) {
        move({x, y});
    }

    // bool collides_with(const Hitbox* hitbox) {
    //     return hitbox_.collides_with(hitbox);
    // }

    // bool collides_with(const HitboxObject& obj) {
    //     return hitbox_.collides_with(&obj.hitbox_);
    // }
};
