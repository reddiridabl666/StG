#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

class BulletGenerator {
  private:
    // sf::Vector2f pos_ = {0, 0};
    std::list<Bullet*> bullets;
    std::unordered_map<std::string, BulletInfo> bullet_types;

  public:
    static std::unordered_set<BulletGenerator*> all;
    static void update_all() {
        for (auto it : all) {
            it->update();
        }
    }

    std::list<Bullet*> getBullets() {
        return bullets;
    };

    explicit BulletGenerator(/*sf::Vector2f pos = {0, 0}*/)/*: pos_(pos)*/ {
        all.insert(this);
    }

    void add_bullet(std::string name, const BulletInfo& info) {
        bullet_types[name] = info;
    }
    
    void shoot(std::string name, sf::Vector2f pos = {0, 0}) {
        auto bullet = new Bullet(bullet_types[name]);
        bullet->setPosition(pos);
        bullets.push_back(bullet);
    }  

    void for_each(std::function<void(Bullet*)> action) {
        for (auto it : bullets) {
            if (it)
                action(it);
        }
    }

    void update() {
        if (bullets.empty()) {
            return;
        }
        // size_t deleted_num = 0;
        for (auto it = bullets.begin(); it != bullets.end();) {
            auto next = std::next(it);
            auto bullet = *it;
            if (bullet) {
                bullet->update_(bullet);
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

    // sf::Vector2f getPosition() {
    //     return pos_;
    // }

    // void setPosition(sf::Vector2f pos) {
    //     pos_ = pos;
    // }

    ~BulletGenerator() {
        for_each([](Bullet* it) {delete it;});
        all.erase(this);
    }
};

inline std::unordered_set<BulletGenerator*> BulletGenerator::all;
