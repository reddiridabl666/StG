#pragma once

#include <list>
#include <unordered_map>
#include "Bullet.hpp"

class BulletGenerator {
    std::list<Bullet> bullets;
    void shoot(std::string bullet_name);  // Make bullet names a enum or something??
};
