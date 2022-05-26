#pragma once

#include "ShootingObject.hpp"
#include "Text.hpp"
#include "GameState.hpp"

class Enemy : public ShootingObject<EnemyBulletGen>, public DamageDealing {
public:
    Enemy(const sf::Texture& texture, const sf::Vector2f& pos, 
          const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) : 
        ShootingObject(texture, pos, hitbox_size, {}, 0, layer), DamageDealing() {
        setHP(hp);
        setTag(Tag::Enemy);
    }

    // void on_collide(DynamicObject* obj) override {
    //     if (obj->getTag() == Tag::PlayerBullet) {
    //         // auto bullet = dynamic_cast<Bullet*>(obj);
    //         dynamic_cast<Bullet*>(obj)->dealDamage(*this);
    //     }
    // }

    void on_collide(Bullet* obj) {
        if (obj && obj->getTag() == Tag::PlayerBullet) {
            // auto bullet = dynamic_cast<Bullet*>(obj);
            obj->dealDamage(*this);
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

class Boss : public Enemy {
protected:
    Text health_bar_;
public:
    Boss(const sf::Texture& texture, const sf::Vector2f& pos, 
          const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) : 
        Enemy(texture, pos, hitbox_size, hp, layer), 
        health_bar_("Health: " + std::to_string(hp),
                    getPosition() + sf::Vector2f{0, -getSize().y}) {
    }

    void update(float deltaTime) override {
        ShootingObject::update(deltaTime);
        health_bar_.setString("Health: " + std::to_string(hp_));
        health_bar_.setPosition(getPosition() + sf::Vector2f{0, -getSize().y});
    }
};

class TestBoss : public Boss {
protected:
    int shot_num = 0;
public:
    using Boss::Boss;
    void shoot(std::string name) override {
        // auto bullet = Bullet::BulletTypes[name];
        sf::Vector2f bullet_size = {150, 150};

        srand(time(nullptr));
        static size_t size = 5;
        sf::Vector2f start_pos = getPosition() - (size / 2.f) * sf::Vector2f{bullet_size.x, 0};
        sf::Vector2f delta = {bullet_size.x, 0}; /* GameState::window()->getView().getSize().x / (size + 1); */
        sf::Vector2f offset = {};

        for (size_t i = 0; i < size; ++i) {
            gen_.shoot(Bullet::BulletTypes[name], 
                           start_pos + offset, 
                           {rand() % 600 - 300.f, rand() % 100 + 25.f},
                           bullet_size);
            offset += delta;
        }

        shot_num++;
    }

    void update(float deltaTime) override {
        Boss::update(deltaTime);
        auto time = shoot_clock_.getElapsedTime().asSeconds();

        if (shot_num < 5 && time >= 1.2) {
            shoot("test_circle");
            shoot_clock_.restart();
        } else  if (time >= 7) {
            shot_num = 0;
            gen_.for_each([](Bullet* it) {it->deactivate();});
        }
    }
};
