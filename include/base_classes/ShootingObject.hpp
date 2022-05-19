#pragma once

#include "BulletGenerator.hpp"

template <typename GeneratorType>
class ShootingObject : public DynamicObject {
  protected:
    GeneratorType gen_;
    sf::Clock shoot_clock_;

  public:
    using DynamicObject::DynamicObject;
    virtual void shoot(std::string name) = 0;
};
