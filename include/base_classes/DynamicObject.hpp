#pragma once

#include "SpriteObject.h"
#include "Hitbox.hpp"

#include <functional>
#include <set>

enum class Tag {
    None,
    PlayerBullet,
    Player,
    Enemy,
    Bullet,
    Wall
};

class DynamicObject : public SpriteObject {
  protected:
    float mass_ = 0;
    sf::Vector2f velocity_ = {0, 0};
    HitboxPtr hitbox_;
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

    static void refresh_collision_num();

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

    virtual void setRotation(float angle) {
        SpriteObject::setRotation(angle);
        if (hitbox_)
            hitbox_->getTransformable()->setRotation(angle);
    }

    void rotate(float angle) {
        setRotation(getRotation() + angle);
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

    void update(float deltaTime) override {
        move(velocity_ * deltaTime);
    }

    bool collides_with(const DynamicObject& obj) {
        return collides_with(&obj);
    }

    bool collides_with(const DynamicObject* obj) {
        if (hitbox_ && obj)
            return hitbox_->collides_with(obj->hitbox_.get());
        return false;
    }

    virtual void on_collide(DynamicObject* obj);

    virtual void on_collide_stop() {
        if (hitbox_)
            hitbox_->on_collide_stop();
    }

    void setHitbox(const HitboxInfo& info) {
        hitbox_ = Hitbox::getHitbox(info, getPosition());
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
        return hitbox_.get();
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
        all.erase(this);
    }
};
