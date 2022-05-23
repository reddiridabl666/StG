#pragma once

#include "SpriteObject.h"
#include "Hitbox.hpp"

#include <functional>

class DynamicObject : public SpriteObject {
  protected:
    float mass_ = 0;
    sf::Vector2f velocity_ = {0, 0};
    Hitbox* hitbox_ = nullptr;
    Tag tag_ = Tag::None;

  public:
    DynamicObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
        sf::Vector2f velocity = {0, 0}, float mass = 0,
        Layer layer = Layer::Character) 
        : SpriteObject(texture, pos, layer), mass_(mass), velocity_(velocity) {
            all.insert(this);
    }

  public:
    static std::set<DynamicObject*> all;   
    static void check_collisions_with(DynamicObject& other);
    static void check_collisions();
    static void move_all(float deltaTime);
    static void refresh_collision_num();
    static void for_each(std::function<void(DynamicObject*)> action);

    explicit DynamicObject(Layer layer = Layer::Character) : SpriteObject(layer) {all.insert(this);}

    DynamicObject(const DynamicObject& rhs);
    DynamicObject& operator=(const DynamicObject& rhs);

    DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                 const HitboxInfo& hitbox_size, sf::Vector2f velocity = {0, 0}, 
                 float mass = 0, Layer layer = Layer::Character);

    virtual void setPosition(const sf::Vector2f& pos) {
        SpriteObject::setPosition(pos);
        if (hitbox_)
            hitbox_->getTransformable()->setPosition(pos);
    }

    void setPosition(float x, float y) {
        setPosition({x, y});
    }

    void show() override {
        GameObject::show();
#ifdef DEBUG
        if (hitbox_) hitbox_->show();
#endif
    }

    void swap(DynamicObject& other);

    void hide() override {
        GameObject::hide();
        if (hitbox_) hitbox_->hide();
    }

    void move(const sf::Vector2f& offset) {
        setPosition(getPosition() + offset);
    }

    void move(float x, float y) {
        move({x, y});
    }

    bool collides_with(const DynamicObject& obj) {
        return collides_with(&obj);
    }

    bool collides_with(const DynamicObject* obj) {
        if (hitbox_ && obj)
            return hitbox_->collides_with(obj->hitbox_);
        return false;
    }

    virtual void on_collide(DynamicObject* obj);

    virtual void on_collide_stop() {
        if (hitbox_)
            hitbox_->on_collide_stop();
    }

    void setHitbox(sf::Vector2f size) {
        if (hitbox_) {
            delete hitbox_;
        }
        hitbox_ = new RectHitbox(size, getPosition());
    }

    void setHitbox(float radius) {
        if (hitbox_) {
            delete hitbox_;
        }
        hitbox_ = new CircleHitbox(radius, getPosition());
    }

    void setVelocity(sf::Vector2f velocity) {
        velocity_ = velocity;
    }

    void setVelocity(float x, float y) {
        setVelocity({x, y});
    }

    void setMass(float mass) {
        mass_ = mass;
    }

    void setTag(Tag tag) {
        tag_ = tag;
    }

    const Hitbox* getHitbox() const {
        return hitbox_;
    }

    Hitbox* getHitbox() {
        return const_cast<Hitbox*>(const_cast<const DynamicObject*>(this)->getHitbox());
    }

    Tag getTag() const {
        return tag_;
    }

    float getMass() const {
        return mass_;
    }

    sf::Vector2f getVelocity() const {
        return velocity_;
    }

    virtual ~DynamicObject() {
        if (hitbox_) {
            delete hitbox_;
        }
        all.erase(this);
    }
};
