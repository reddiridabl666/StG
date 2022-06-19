#pragma once

#include <variant>

#include "BulletUtils.hpp"
#include "FramedObject.hpp"
#include "Damage.hpp"

class Bullet : public FramedObject, public DamageDealing {
public:
    struct Info {
        const sf::Texture* texture = nullptr;
        HitboxInfo hitbox_info = 0;
        sf::Vector2f velocity = {0, 0};
        UpdateFunc update;
        int damage = 1;
        float mass = 0;
        sf::Vector2f size = {0, 0};
        HitboxInfo frame_info = 0;
    };
protected:
    sf::Clock clock;
    UpdateFunc update_ = UpdateFunc([] (Bullet*, float) {});

    static std::unordered_map<BulletType, sf::Texture> getBulletTextures();
    static std::unordered_map<BulletType, Info> getBulletTypes(); 

public:
    static std::unordered_map<BulletType, sf::Texture> textures;
    static std::unordered_map<BulletType, Info> Types; 

    Bullet(Layer layer = Layer::Bullet) : FramedObject(layer) {}

    Bullet(Info info, Layer layer = Layer::Bullet);
    Bullet(const Bullet&);
    Bullet& operator=(const Bullet&);

    void setUpdateFunc(const UpdateFunc& new_func) {
        update_ = new_func;
    }

    UpdateFunc& updateFunc() {
        return update_;
    }
    
    void setSize(const sf::Vector2f& size) {
        scale(size.x / getSize().x, size.y / getSize().y);
    }

    sf::Time getTime() const {
        return clock.getElapsedTime();
    }

    template<typename> friend class BulletGenerator;

    friend class Game;
};

// inline const UpdateFunc gravity([] (Bullet* bullet, float deltaTime) {
//     auto y = bullet->getVelocity().y + constants::g * deltaTime;
//     auto x = bullet->getVelocity().x;
//     bullet->setVelocity(x, y);
// });
