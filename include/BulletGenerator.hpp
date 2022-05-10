#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "Bullet.hpp"

class BulletGenerator {
  private:
    std::list<Bullet*> bullets;
    std::unordered_map<std::string, BulletInfo> bullet_types;

  public:
    static std::unordered_set<BulletGenerator*> all;
    static void update_all() {
        for (auto it : all) {
            it->update();
        }
    }

    BulletGenerator() {
        all.insert(this);
    }

    void add_bullet(std::string name, const BulletInfo& info) {
        bullet_types[name] = info;
    }
    
    void shoot(std::string name) { 
        bullets.push_back(new Bullet(bullet_types[name]));
    }  

    void for_each(std::function<void(Bullet*)> action) {
        for (auto it : bullets) {
            action(it);
        }
    }

    void update() {
        for (auto it = bullets.begin(); it != bullets.end(); ++it) {
            auto bullet = *it;
            bullet->update();
            if (!bullet->is_active()) {
                delete bullet;
                bullet = nullptr;
                bullets.erase(it);
            }
        }
    }

    ~BulletGenerator() {
        for_each([](Bullet* it) {if (it) delete it;});
        all.erase(this);
    }
};

inline std::unordered_set<BulletGenerator*> BulletGenerator::all;
