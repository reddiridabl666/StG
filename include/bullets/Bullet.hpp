#pragma once

#include <variant>
// #include "DynamicObject.hpp"
#include "FramedObject.hpp"
#include "Damage.hpp"

class Bullet;

// using UpdateFunc = std::function<void(Bullet*, float)>;

class UpdateFunc {
private:
    std::function<void(Bullet*, float)> func_;
public:
    UpdateFunc() : func_() {}
    UpdateFunc(const std::function<void(Bullet*, float)>& func) : func_(func) {}

    void operator()(Bullet* bullet, float time) const {
        func_(bullet, time);
    }
    
    UpdateFunc operator+(const UpdateFunc& other) const {
        return UpdateFunc([this, other](Bullet* bullet, float time) {
            (*this)(bullet, time);
            other(bullet, time);
        });
    }
};

struct BulletInfo {
    const sf::Texture* texture = nullptr;
    // const sf::Texture texture;
    HitboxInfo hitbox_info = 0;
    sf::Vector2f velocity = {0, 0};
    const UpdateFunc update;
    int damage = 1;
    // sf::Vector2f size = static_cast<sf::Vector2f>(texture->getSize());
    float mass = 0;
    HitboxInfo phys_info = 0;
    // std::string name;
};

class Bullet : public /* DynamicObject */FramedObject, public DamageDealing {
protected:
    // size_t damage_;
    sf::Clock clock;
    UpdateFunc update_ = UpdateFunc([] (Bullet*, float) {});

    static std::unordered_map<std::string, sf::Texture> getBulletTextures();
    // static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, BulletInfo> getBulletTypes(); 

public:
static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, BulletInfo> BulletTypes; 

    Bullet(Layer layer = Layer::Bullet) : /* DynamicObject */FramedObject(layer) {}

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

    // friend class BulletGenerator;
    template<typename> friend class BulletGenerator;
};
