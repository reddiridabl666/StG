#pragma once

#include <variant>
#include "DynamicObject.hpp"

class Bullet;

using HitboxInfo = std::variant<sf::Vector2f, float>;
using UpdateFunc = std::function<void(Bullet*, float)>;

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    // const sf::Texture texture;
    HitboxInfo hitbox_info = 0;
    sf::Vector2f velocity = {0, 0};
    const UpdateFunc* update = nullptr;
    float mass = 0;
    // std::string name;
};

class Bullet : public DynamicObject {
protected:
    sf::Clock clock;
    UpdateFunc update_ = [] (Bullet*, float) {};

    static std::unordered_map<std::string, sf::Texture> getBulletTextures();
    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, BulletInfo> getBulletTypes(); 

public:
    static std::unordered_map<std::string, BulletInfo> BulletTypes; 

    Bullet(Layer layer = Layer::Bullet) : DynamicObject(layer) {
        clock.restart();  // I think it isn't needed?
    }

    Bullet(BulletInfo info, Layer layer = Layer::Bullet);
    Bullet(const Bullet&);
    Bullet& operator=(const Bullet&);

    void setUpdateFunc(const UpdateFunc& new_func) {
        update_ = new_func;
    }

    sf::Time getTime() {
        return clock.getElapsedTime();
    }

    friend class BulletGenerator;
};

inline std::unordered_map<std::string, sf::Texture> Bullet::textures;
