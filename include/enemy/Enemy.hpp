#pragma once

#include "ShootingObject.hpp"
#include "Text.hpp"
#include "Player.hpp"

// #include "GameState.hpp"
// #include "BulletTypes.hpp"

class EnemyBullet;

class Enemy : public ShootingObject<EnemyBullet>, public DamageDealing {
public:
    Enemy(const sf::Texture& texture, const sf::Vector2f& pos, 
          const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) : 
        ShootingObject(texture, pos, hitbox_size, {}, 0, layer), DamageDealing() {
        setHP(hp);
        setTag(Tag::Enemy);
    }

    void on_collide(Bullet* obj) {
        if (obj && obj->getTag() == Tag::PlayerBullet) {
            obj->dealDamage(*this);
            obj->deactivate();
        }
    }

    void player_collision(Player* player) {
        if (!player) {
            return;
        }
        if (frame_hitbox_->collides_with(player->getHitbox())) {
            player->on_collide(this);
        }
        for (auto it : player->getBullets()) {
            if (collides_with(it)) {
                on_collide(it);
                it->on_collide(this);
            }
        }
    }
};

class EnemyBullet : public Bullet {
public:
    EnemyBullet(Layer layer = Layer::Bullet) : Bullet(layer) {}

    EnemyBullet(BulletInfo info, Layer layer = Layer::Bullet) : Bullet(info, layer) {
        setTag(Tag::Enemy);
    }
};
