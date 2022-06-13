#pragma once

#include "BulletGenerator.hpp"
#include "HealthObject.hpp"

template <typename T>
class ShootingObject : public HealthObject {
protected:
    BulletGenerator<T> gen_;
    sf::Clock shoot_clock_;
    float shot_interval = 0.1;

public:
    // using FramedObject::FramedObject;
    using HealthObject::HealthObject;

    void check_bullet_collisions(HealthObject* obj) {
        if (!obj) {
            return;
        }
        for (auto& it : getBullets()) {
            if (it->collides_with(obj)) {
                it->on_collide(obj);
                obj->on_collide(it.get());
            } 
        }
    }

    std::list<BulletPtr>& getBullets() {
        return gen_.getBullets();
    }
    
    void update(float deltaTime) override {
        HealthObject::update(deltaTime);
        gen_.update(deltaTime);
    }

    virtual void shoot(BulletType name) {
        gen_.shoot(Bullet::Types[name], getPosition());
    }

    friend struct Phase;
};
