#pragma once

#include "BulletGenerator.hpp"

template <typename GeneratorType>
class ShootingObject : public FramedObject {
protected:
    GeneratorType gen_;
    sf::Clock shoot_clock_;
    float shot_interval = 0.1;

public:
    // using DynamicObject::DynamicObject;
    using FramedObject::FramedObject;
    virtual void shoot(std::string name) = 0;
};
