#pragma once

#include <variant>
#include "DynamicObject.hpp"

using HitboxInfo = std::variant<sf::Vector2f, float>;
class Bullet;

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    // const sf::Texture texture;
    HitboxInfo hitbox_info = 0;
    sf::Vector2f velocity = {0, 0};
    const std::function<void(Bullet*)>* update = nullptr;
    float mass = 0;
    // std::string name;
};

class Bullet : public DynamicObject {
protected:
    sf::Clock clock;
    std::function<void(Bullet*)> update_ = [] (Bullet*) {};
    static void init_textures();

public:
    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, BulletInfo> getBulletTypes();

    Bullet(Layer layer = Layer::Bullet) : DynamicObject(layer) {
        clock.restart();  // I think it isn't needed?
    }

    Bullet(BulletInfo info, Layer layer = Layer::Bullet);

    Bullet(const Bullet&);

    Bullet& operator=(const Bullet&);

    void setUpdateFunc(const std::function<void(Bullet* it)>& new_func) {
        update_ = new_func;
    }

    friend class BulletGenerator;
};
