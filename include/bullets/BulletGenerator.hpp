#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

#include "HealthObject.hpp"

// class BulletGeneratorBase {
// protected:
//     static std::unordered_set<BulletGeneratorBase*> all;
//     std::list<Bullet*> bullets_;

//     BulletGeneratorBase() {
//         all.insert(this);
//     }

//     virtual ~BulletGeneratorBase() {
//         all.erase(this);
//     }

//     virtual void update(float) = 0;

//     // TODO: МБ надо вообще убрать этот класс и делать сеты генераторов для каждого типа пуль??
// public:
//     static void check_collisions_with_bullets(HealthObject* obj) {
//         if (!obj) {
//             return;
//         }
        
//         for (const auto gen : all) {
//             for (auto bullet : gen->bullets_) {
//                 if (bullet->collides_with(obj)) {
//                     obj->on_collide(bullet);
//                     bullet->on_collide(obj);
//                 }
//             }
            
//             // Мб запихнуть проверку на отсутствие коллизий
//         }
//     }

//     template<typename> friend class BulletGenerator;
// };

template <typename BulletType>
class BulletGenerator /* : public BulletGeneratorBase */ {
  private:
    std::list<Bullet*> bullets_;
    // sf::Vector2f pos_ = {0, 0};
    // std::list<Bullet*> bullets_;
    // std::unordered_map<std::string, BulletInfo> bullet_types;

  public:
    // static std::unordered_set<BulletGenerator*> all;

    // static void update_all(float deltaTime) {
    //     for (auto it : all) {
    //         if (it)
    //             it->update(deltaTime);
    //     }
    // }

    std::list<Bullet*>& getBullets() {
        return bullets_;
    }

    BulletGenerator(/*sf::Vector2f pos = {0, 0}*/)/*: pos_(pos)*/ {
        // all.insert(this);
    }

    virtual Bullet* shoot(const BulletInfo& info) {
        auto bullet = new BulletType(info);
        bullets_.push_back(bullet);
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

//     Bullet* shoot(const BulletInfo& info) override {
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

//     Bullet* shoot(const BulletInfo& info) override {
//         auto bullet = BulletGenerator::shoot(info);
//         bullet->setTag(Tag::PlayerBullet);
//         // std::cout << "Player bullet shot!\n";
//         return bullet;
//     }
// };
