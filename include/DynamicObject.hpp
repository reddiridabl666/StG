#pragma once

#include "GameObject.h"
#include "Hitbox.h"

#include <functional>

class DynamicObject : public GameObject {
  protected:
    float mass_ = 0;
    sf::Vector2f velocity_ = {0, 0};
    Hitbox* hitbox_ = nullptr;
    Tag tag_ = Tag::None;

  public:
    DynamicObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
        sf::Vector2f velocity = {0, 0}, float mass = 0,
        Layer layer = Layer::Character) 
        : GameObject(texture, pos, layer), mass_(mass), velocity_(velocity) {
            all.insert(this);
    }

  public:
    static std::set<DynamicObject*> all;   
    static void check_collisions_with(DynamicObject& other);
    static void check_collisions();
    static void move_all(float deltaTime);
    static void refresh_collision_num();
    static void for_each(std::function<void(DynamicObject*)> action);

    explicit DynamicObject(Layer layer = Layer::Character) : GameObject(layer) {all.insert(this);}

    DynamicObject(const DynamicObject& rhs) : DynamicObject(*rhs.getTexture(), rhs.getPosition(), 
                                                            rhs.velocity_, rhs.mass_, rhs.layer_) 
    {
        // TODO: Избавиться от каста, добавив виртуальный метод
        if (auto rect = dynamic_cast<const RectHitbox*>(rhs.getHitbox())) {
            hitbox_ = new RectHitbox(rect->getSize(), getPosition());
            return;
        }
        if (auto circle = dynamic_cast<const CircleHitbox*>(rhs.getHitbox())) {
            hitbox_ = new CircleHitbox(circle->getRadius(), getPosition());
            return;
        }
    }

    DynamicObject& operator=(const DynamicObject& rhs) {
        // TODO: Избавиться от каста, добавив виртуальный метод
        if (rhs.getTexture())
            setTexture(*rhs.getTexture());
        
        if (hitbox_) {
            delete hitbox_;
        }

        setVelocity(rhs.velocity_);
        setMass(rhs.mass_);
        change_layer(rhs.layer_);

        if (auto rect = dynamic_cast<const RectHitbox*>(rhs.getHitbox())) {
            hitbox_ = new RectHitbox(rect->getSize());
        }
        if (auto circle = dynamic_cast<const CircleHitbox*>(rhs.getHitbox())) {
            hitbox_ = new CircleHitbox(circle->getRadius());
        }

        setPosition(rhs.getPosition());
        return *this;
    }

    DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                 const sf::Vector2f& hitbox_size, sf::Vector2f velocity = {0, 0}, 
                 float mass = 0, Layer layer = Layer::Character);

    DynamicObject(const sf::Texture& texture, sf::Vector2f pos, float hitbox_radius, 
        sf::Vector2f velocity = {0, 0}, float mass = 0, Layer layer = Layer::Character);

    void setPosition(const sf::Vector2f& offset) {
        // TODO: Избавиться от каста, добавив get_transformable в базу

        GameObject::setPosition(offset);
        if (hitbox_)
            dynamic_cast<sf::Transformable*>(hitbox_)->setPosition(offset);
    }

    void setPosition(float x, float y) {
        setPosition({x, y});
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
