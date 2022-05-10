#pragma once

#include <variant>
#include "DynamicObject.hpp"

using HitboxInfo = std::variant<sf::Vector2f, float>;

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    HitboxInfo hitbox_info;
    sf::Vector2f velocity = {0, 0};
};

class BulletGenerator;

class Bullet : public DynamicObject {
  protected:
    sf::Clock clock;
    std::function<void()> update = [this] () {};

  public:
    Bullet(Layer layer = Layer::Bullet) : DynamicObject(layer) {
        clock.restart();  // I think it isn't needed?
    }

    Bullet(BulletInfo info, Layer layer = Layer::Bullet) : Bullet(layer) {
        if (info.texture) {
            setTexture(*info.texture);
        }
        if (auto size = std::get_if<sf::Vector2f>(&info.hitbox_info)) {
            hitbox_ = new RectHitbox(*size);
        } else if (auto size = std::get_if<float>(&info.hitbox_info)) {
            hitbox_= new CircleHitbox(*size);
        }
        setVelocity(info.velocity);
    }

    Bullet(const Bullet& other) : 
        Bullet(BulletInfo{other.getTexture(), 
                        dynamic_cast<const CircleHitbox*>(other.getHitbox()) ? 
                        HitboxInfo(other.getHitbox()->getHalfSize().x) : 
                        HitboxInfo(other.getHitbox()->getSize()),
                        other.getVelocity()}) {}

    Bullet& operator=(const Bullet& other) {
        clock.restart();
        setUpdateFunc(other.update);
        DynamicObject::operator=(other);
        return *this;
    }

    void setUpdateFunc(const std::function<void()>& new_func) {
        update = new_func;
    }

    friend BulletGenerator;
};
