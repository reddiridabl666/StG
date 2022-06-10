#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

#include "HealthObject.hpp"

template <typename T>
class BulletGenerator {
  private:
    std::list<Bullet*> bullets_;

  public:
    std::list<Bullet*>& getBullets() {
        return bullets_;
    }

    BulletGenerator() = default;

    virtual Bullet* shoot(const Bullet::Info& info) {
        auto bullet = new T(info);
        if (info.size != sf::Vector2f{0, 0})
            bullet->setSize(info.size);
        bullets_.push_back(bullet);
        return bullet;
    }

    Bullet* shoot(const Bullet::Info& info, sf::Vector2f pos) {
        auto bullet = shoot(info);
        bullet->setPosition(pos);
        return bullet;
    }    

    Bullet* shoot(const Bullet::Info& info, sf::Vector2f pos, sf::Vector2f velocity) {
        auto bullet = shoot(info, pos);
        bullet->setVelocity(velocity);
        return bullet;
    }

    void for_each(const std::function<void(Bullet*)>& action) {
        for (auto it : bullets_) {
            if (it)
                action(it);
        }
    }

    void update(float deltaTime) {
        if (bullets_.empty()) {
            return;
        }
        // size_t deleted_num = 0;
        for (auto it = bullets_.begin(); it != bullets_.end();) {
            auto next = std::next(it);
            auto bullet = *it;
            if (bullet) {
                bullet->update_(bullet, deltaTime);
                if (bullet->is_active() == false) {
                    // ++deleted_num;
                    delete bullet;
                    bullet = nullptr;
                    bullets_.erase(it);
                }
            }
            it = next;
        }
        // if (deleted_num > 0 ) std::cout << deleted_num << " bullets were deleted\n";
    }

    virtual ~BulletGenerator() {
        for_each([](Bullet* it) {delete it;});
        // all.erase(this);
    }
};

// TODO: Переместить?
// inline std::unordered_set<BulletGeneratorBase*> BulletGeneratorBase::all;

// class EnemyBulletGen : public BulletGenerator {
// public:
//     using BulletGenerator::BulletGenerator;
//     using BulletGenerator::shoot;

//     Bullet* shoot(const Bullet::Info& info) override {
//         auto bullet = BulletGenerator::shoot(info);
//         bullet->setTag(Tag::Enemy);
//         // std::cout << "Enemy bullet shot!\n";
//         return bullet;
//     }
// };

// class PlayerBulletGen : public BulletGenerator {
// public:
//     using BulletGenerator::BulletGenerator;
//     using BulletGenerator::shoot;

//     Bullet* shoot(const Bullet::Info& info) override {
//         auto bullet = BulletGenerator::shoot(info);
//         bullet->setTag(Tag::PlayerBullet);
//         // std::cout << "Player bullet shot!\n";
//         return bullet;
//     }
// };
