#pragma once

#include "DynamicObject.hpp"
struct BulletInfo {
    sf::Texture* texture = nullptr;
    Hitbox* hitbox = nullptr;
    sf::Vector2f velocity = {0, 0};
};

class Bullet : public DynamicObject {
    using DynamicObject::DynamicObject;
    // virtual void trajectory_update() {};
};
