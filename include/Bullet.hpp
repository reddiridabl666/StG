#pragma once

#include <variant>
#include "DynamicObject.hpp"

using HitboxInfo = std::variant<sf::Vector2f, float>;
class Bullet;

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    HitboxInfo hitbox_info = 0;
    sf::Vector2f velocity = {0, 0};
    std::function<void(Bullet*)>* update = nullptr;
    float mass = 0;
};

class Bullet : public DynamicObject {
  protected:
    sf::Clock clock;
    std::function<void(Bullet*)> update_ = [] (Bullet*) {};

  public:
    Bullet(Layer layer = Layer::Bullet) : DynamicObject(layer) {
        clock.restart();  // I think it isn't needed?
    }

    Bullet(BulletInfo info, Layer layer = Layer::Bullet) : Bullet(layer) {
        if (info.texture) {
            setTexture(*info.texture);
        }
        if (info.update) {
            setUpdateFunc(*info.update);
        }
        if (auto size = std::get_if<sf::Vector2f>(&info.hitbox_info)) {
            hitbox_ = new RectHitbox(*size);
        } else if (auto size = std::get_if<float>(&info.hitbox_info)) {
            hitbox_= new CircleHitbox(*size);
        }
        setVelocity(info.velocity);
        setMass(info.mass);
    }

    Bullet(const Bullet& other) : 
        Bullet(BulletInfo{other.getTexture(), 
                        dynamic_cast<const CircleHitbox*>(other.getHitbox()) ? 
                        HitboxInfo(other.getHitbox()->getHalfSize().x) : 
                        HitboxInfo(other.getHitbox()->getSize()),
                        other.getVelocity()}) {}

    Bullet& operator=(const Bullet& other) {
        clock.restart();
        setUpdateFunc(other.update_);
        DynamicObject::operator=(other);
        return *this;
    }

    void setUpdateFunc(const std::function<void(Bullet* it)>& new_func) {
        update_ = new_func;
    }

    friend class BulletGenerator;
};
