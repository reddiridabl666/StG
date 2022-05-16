#pragma once

#include "BulletGenerator.hpp"

class ShootingObject : public DynamicObject {
  protected:
    BulletGenerator gen_;
    sf::Clock clock_;

  public:
    using DynamicObject::DynamicObject;
    
    void add_bullet(std::string name, const BulletInfo& info) {
        gen_.add_bullet(name, info);
    }

    virtual void shoot(std::string name) = 0;
};