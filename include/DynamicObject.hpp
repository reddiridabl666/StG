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

  private:
    DynamicObject(const sf::Texture& texture, sf::Vector2f pos,
        sf::Vector2f velocity, float mass,
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

    explicit DynamicObject(Layer layer = Layer::Character) : GameObject(layer) {}

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

        setTexture(*rhs.getTexture());
        setVelocity(rhs.velocity_);
        setMass(rhs.mass_);
        change_layer(rhs.layer_);
        if (auto rect = dynamic_cast<const RectHitbox*>(rhs.getHitbox())) {
            hitbox_ = new RectHitbox(rect->getSize());
            return *this;
        }
        if (auto circle = dynamic_cast<const CircleHitbox*>(rhs.getHitbox())) {
            hitbox_ = new CircleHitbox(circle->getRadius());
            return *this;
        }
        setPosition(rhs.getPosition());
    }

    DynamicObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, 
                 const sf::Vector2f& hitbox_size = {35, 35}, sf::Vector2f velocity = {0, 0}, 
                 float mass = 0, Layer layer = Layer::Character);

    DynamicObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, float hitbox_radius = 20, 
        sf::Vector2f velocity = {0, 0}, float mass = 0, Layer layer = Layer::Character);

    void setPosition(const sf::Vector2f& offset) {
        // TODO: Избавиться от каста, добавив get_transformable в базу

        GameObject::setPosition(offset);
        if (hitbox_ != nullptr)
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

    virtual void on_collide(const DynamicObject* obj);

    virtual void on_collide_stop() {
        if (hitbox_)
            hitbox_->on_collide_stop();
    }

    void setHitbox(sf::Vector2f size) {
        if (hitbox_) {
            delete hitbox_;
        }
        hitbox_ = new RectHitbox(size);
    }

    void setHitbox(float radius) {
        if (hitbox_) {
            delete hitbox_;
        }
        hitbox_ = new CircleHitbox(radius);
    }

    void setVelocity(sf::Vector2f velocity) {
        velocity_ = velocity;
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

    Tag getTag() const {
        return tag_;
    }

    virtual ~DynamicObject() {
        if (hitbox_) {
            delete hitbox_;
        }
        all.erase(this);
    }
};
