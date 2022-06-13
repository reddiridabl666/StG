#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

#include "HealthObject.hpp"

using BulletPtr = std::unique_ptr<Bullet>;

template <typename T>
class BulletGenerator {
  private:
    std::list<BulletPtr> bullets_;

  public:
    std::list<BulletPtr>& getBullets() {
        return bullets_;
    }

    BulletGenerator() = default;

    virtual BulletPtr& shoot(const Bullet::Info& info) {
        auto bullet = std::make_unique<T>(info);
        if (info.size != sf::Vector2f{0, 0})
            bullet->setSize(info.size);
        bullets_.push_back(std::move(bullet));
        return bullets_.back();
    }

    BulletPtr& shoot(const Bullet::Info& info, sf::Vector2f pos) {
        auto& bullet = shoot(info);
        bullet->setPosition(pos);
        return bullet;
    }    

    BulletPtr& shoot(const Bullet::Info& info, sf::Vector2f pos, sf::Vector2f velocity) {
        auto& bullet = shoot(info, pos);
        bullet->setVelocity(velocity);
        return bullet;
    }

    BulletPtr& shoot(const Bullet::Info& info, sf::Vector2f pos, sf::Vector2f velocity, float angle) {
        auto& bullet = shoot(info, pos, velocity);
        bullet->setRotation(angle);
        return bullet;
    }

    void for_each(const std::function<void(BulletPtr&)>& action) {
        for (auto& it : bullets_) {
            if (it)
                action(it);
        }
    }

    void update(float deltaTime) {
        if (bullets_.empty()) {
            return;
        }

        for (auto it = bullets_.begin(); it != bullets_.end();) {
            auto& bullet = *it;

            if (bullet) {
                bullet->update_(bullet.get(), deltaTime);

                if (!bullet->is_active()) {
                    it = bullets_.erase(it);
                    continue;
                }
            }

            ++it;
        }
    }
};
