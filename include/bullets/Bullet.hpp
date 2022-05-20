#pragma once

#include <variant>
#include "DynamicObject.hpp"
#include "Damage.hpp"

class Bullet;

using HitboxInfo = std::variant<sf::Vector2f, float>;
using UpdateFunc = std::function<void(Bullet*, float)>;

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    // const sf::Texture texture;
    HitboxInfo hitbox_info = 0;
    sf::Vector2f velocity = {0, 0};
    const UpdateFunc* update = nullptr;
    size_t damage = 1;
    // sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
    float mass = 0;
    // std::string name;
};

class Bullet : public DynamicObject, public DamageDealing {
protected:
    // size_t damage_;
    sf::Clock clock;
    UpdateFunc update_ = [] (Bullet*, float) {};

    static std::unordered_map<std::string, sf::Texture> getBulletTextures();
    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, BulletInfo> getBulletTypes(); 

public:
    static std::unordered_map<std::string, BulletInfo> BulletTypes; 

    Bullet(Layer layer = Layer::Bullet) : DynamicObject(layer) {}

    Bullet(BulletInfo info, Layer layer = Layer::Bullet);
    Bullet(const Bullet&);
    Bullet& operator=(const Bullet&);

    void setUpdateFunc(const UpdateFunc& new_func) {
        update_ = new_func;
    }
    
    void setSize(const sf::Vector2f& size) {
        scale(size.x / getSize().x, size.y / getSize().y);
    }

    sf::Time getTime() const {
        return clock.getElapsedTime();
    }

    friend class BulletGenerator;
};
