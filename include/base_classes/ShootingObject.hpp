#pragma once

#include "BulletGenerator.hpp"
#include "HealthObject.hpp"

template <typename GeneratorType>
class ShootingObject : public HealthObject {
protected:
    GeneratorType gen_;
    sf::Clock shoot_clock_;
    float shot_interval = 0.1;

public:
    // using FramedObject::FramedObject;
    using HealthObject::HealthObject;
    void update(float deltaTime) override {
        HealthObject::update(deltaTime);
        gen_.update(deltaTime);
    }
    virtual void shoot(std::string name) = 0;
};
