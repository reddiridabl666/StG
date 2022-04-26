#pragma once

#include "GameObject.h"
#include "Hitbox.h"

class HitboxObject : public GameObject {
  public:
  // protected:
    Hitbox* hitbox_ = nullptr;

  public:
    HitboxObject(Layer layer = Layer::character) : GameObject(layer) {}

    HitboxObject(const sf::Texture& texture, const sf::Vector2f& size, Layer layer = Layer::character) 
        : GameObject(texture, layer), hitbox_(new RectHitbox(size)) {};

    HitboxObject(const sf::Texture& texture, float radius, Layer layer = Layer::character) 
        : GameObject(texture, layer), hitbox_(new CircleHitbox(radius)) {};

    void setPosition(const sf::Vector2f& offset) {
        GameObject::setPosition(offset);
        dynamic_cast<sf::Transformable*>(hitbox_)->setPosition(offset);
    }

    void setPosition(float x, float y) {
        setPosition({x, y});
    }

    void move(const sf::Vector2f& offset) {
        GameObject::move(offset);
        dynamic_cast<sf::Transformable*>(hitbox_)->move(offset);
    }

    void move(float x, float y) {
        move({x, y});
    }

    bool collides_with(const Hitbox* hitbox) {
        return hitbox_->collides_with(hitbox);
    }

    bool collides_with(const HitboxObject& obj) {
        return hitbox_->collides_with(obj.hitbox_);
    }

    virtual ~HitboxObject() {
        if (hitbox_) {
            delete hitbox_;
        }
    }
};
