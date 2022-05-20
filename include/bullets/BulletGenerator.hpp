#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

class BulletGenerator {
  private:
    // sf::Vector2f pos_ = {0, 0};
    std::list<Bullet*> bullets;
    // std::unordered_map<std::string, BulletInfo> bullet_types;

  public:
    static std::unordered_set<BulletGenerator*> all;

    static void update_all(float deltaTime) {
        for (auto it : all) {
            it->update(deltaTime);
        }
    }

    std::list<Bullet*>& getBullets() {
        return bullets;
    }

    explicit BulletGenerator(/*sf::Vector2f pos = {0, 0}*/)/*: pos_(pos)*/ {
        all.insert(this);
    }

    virtual Bullet* shoot(const BulletInfo& info) {
        auto bullet = new Bullet(info);
        bullets.push_back(bullet);
        return bullet;
    }

    Bullet* shoot(const BulletInfo& info, sf::Vector2f pos) {
        auto bullet = shoot(info);
        bullet->setPosition(pos);
        return bullet;
    }    

    Bullet* shoot(const BulletInfo& info, sf::Vector2f pos, sf::Vector2f velocity) {
        auto bullet = shoot(info, pos);
        bullet->setVelocity(velocity);
        return bullet;
    }

    Bullet* shoot(const BulletInfo& info, sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f size) {
        auto bullet = shoot(info, pos, velocity);
        bullet->setSize(size);
        return bullet;
    }

    void for_each(const std::function<void(Bullet*)>& action) {
        for (auto it : bullets) {
            if (it)
                action(it);
        }
    }

    void update(float deltaTime) {
        if (bullets.empty()) {
            return;
        }
        // size_t deleted_num = 0;
        for (auto it = bullets.begin(); it != bullets.end();) {
            auto next = std::next(it);
            auto bullet = *it;
            if (bullet) {
                bullet->update_(bullet, deltaTime);
                if (bullet->is_active() == false) {
                    // ++deleted_num;
                    delete bullet;
                    bullet = nullptr;
                    bullets.erase(it);
                }
            }
            it = next;
        }
        // if (deleted_num > 0 ) std::cout << deleted_num << " bullets were deleted\n";
    }

    virtual ~BulletGenerator() {
        for_each([](Bullet* it) {delete it;});
        all.erase(this);
    }
};

inline std::unordered_set<BulletGenerator*> BulletGenerator::all;

class EnemyBulletGenerator : public BulletGenerator {
public:
    using BulletGenerator::BulletGenerator;
    using BulletGenerator::shoot;

    Bullet* shoot(const BulletInfo& info) override {
        auto bullet = BulletGenerator::shoot(info);
        bullet->setTag(Tag::Enemy);
        // std::cout << "Enemy bullet shot!\n";
        return bullet;
    }
};
