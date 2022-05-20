#pragma once

#include "ShootingObject.hpp"

class Enemy : public ShootingObject<EnemyBulletGenerator>, public DamageDealing, public Health {
public:
    Enemy(const sf::Texture& texture, const sf::Vector2f& pos, 
          const sf::Vector2f& hitbox_size, size_t hp, Layer layer = Layer::Character) : 
        ShootingObject(texture, pos, hitbox_size, {}, 0, layer), DamageDealing(), Health(hp) {
    }
};

class TestEnemy : public Enemy {
public:
    using Enemy::Enemy;
    
};
