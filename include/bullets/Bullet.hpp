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
        HitboxInfo phys_info = 0;
    };
protected:
    sf::Clock clock;
    UpdateFunc update_ = UpdateFunc([] (Bullet*, float) {});

    static std::unordered_map<BulletType, sf::Texture> getBulletTextures();
    static std::unordered_map<BulletType, Info> getBulletTypes(); 

public:
    static std::unordered_map<BulletType, sf::Texture> textures;
    static std::unordered_map<BulletType, Info> Types; 

    Bullet(Layer layer = Layer::Bullet) : /* DynamicObject */FramedObject(layer) {}

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

    // friend class BulletGenerator;
    template<typename> friend class BulletGenerator;
};
