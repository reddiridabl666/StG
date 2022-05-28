#pragma once

#include "FramedObject.hpp"
#include "Damage.hpp"

class HealthObject : public FramedObject, public Health {
public:
    using FramedObject::FramedObject;

    virtual void on_collide(Bullet* bullet) {
        FramedObject::on_collide(bullet);
        if (!bullet) {
            return;
        }
        bullet->dealDamage(*this);
    }
};
