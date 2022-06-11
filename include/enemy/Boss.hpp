#pragma once

#include "Animated.hpp"
#include "Enemy.hpp"
#include "UpdateFunctions.hpp"

class Boss : public Enemy {
protected:
    void changePhase(Phase* phase) {
        phase_num_++;
        shoot_clock_.restart();
        shot_num_ = 0;
        phase_.reset(phase);
        gen_.for_each([](Bullet* it) {it->deactivate();});
    }

    std::unique_ptr<Phase> phase_;
    Text health_bar_;
    Text phase_left_;
    sf::Uint8 phase_num_ = 0;
    sf::Uint8 phase_max_ = 4;

public:
    Boss(const sf::Texture& texture, const sf::Vector2f& pos, 
          const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) : 
        Enemy(texture, pos, hitbox_size, hp, layer), 
        health_bar_("Health: " + std::to_string(hp),
                    getPosition() + sf::Vector2f{-getHalfSize().x, -getSize().y}),
        phase_left_("Phase: " + std::to_string(phase_num_) + "/" + std::to_string(phase_max_), {50, 50}) {
    }

    void update(float deltaTime) override {
        ShootingObject::update(deltaTime);
        health_bar_.setString("Health: " + std::to_string(hp_));
        health_bar_.setPosition(getPosition() + sf::Vector2f{-getHalfSize().x, -getSize().y});
        phase_left_.setString("Phase: " + std::to_string(phase_num_) + "/" + std::to_string(phase_max_));
    }

    virtual void shoot() = 0;

    friend class Phase;
};

class AnimatedBoss : virtual public Animated, virtual public Boss {
public:
    AnimatedBoss(const Animated::Sprites& sprites, const sf::Vector2f& pos, 
                 const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) :
        Animated(sprites), Boss(sprites_["idle"][0], pos, hitbox_size, hp, layer) {}

    void setTexture(const sf::Texture& texture) override {
        Boss::setTexture(texture);
    }

    void update(float deltaTime) override {
        Boss::update(deltaTime);
        Animated::update();
    }
};

class AudibleBoss : virtual public Boss, public Audible {
public:
    using Boss::Boss;
};

class AudibleAnimatedBoss : public AnimatedBoss, public AudibleBoss {
public:
    AudibleAnimatedBoss(const Animated::Sprites& sprites, const sf::Vector2f& pos, 
                        const HitboxInfo& hitbox_size, size_t hp, Layer layer = Layer::Character) :
        Animated(sprites),
        Boss(sprites_["idle"][0], pos, hitbox_size, hp, layer),
        AnimatedBoss(sprites, pos, hitbox_size, hp, layer),
        AudibleBoss(sprites_["idle"][0], pos, hitbox_size, hp, layer) {}

    using AnimatedBoss::update;
};

struct Phase {
protected:
    Boss* parent;
    size_t num;
    sf::Vector2f start_pos;
    float shot_interval;
    float delta;
    float speed;
public:
    explicit Phase(Boss* parent, int hp = 6000, float interval = 0.5) : parent(parent), shot_interval(interval) {
        parent->setHP(hp);
        start_pos = parent->getPosition();
    }

    int hp() {
        return parent->HP();
    }

    auto& gen() {
        return parent->gen_;
    }

    size_t& shot_num() {
        return parent->shot_num_;
    }
    sf::Clock& shoot_clock() {
        return parent->shoot_clock_;
    }

    virtual void update(float time) {
        if (time >= shot_interval) {
            parent->shoot();
            shoot_clock().restart();
        }
    }

    virtual void shoot() = 0;

    Bullet* shoot_circular(BulletType type, float radius, float angle, sf::Vector2f velocity = {}) {
        float x = start_pos.x + radius * cos(angle);
        float y = start_pos.y + radius * sin(angle);

        velocity = velocity == sf::Vector2f{} ? unit_vector({x, y}, parent->getPosition()) * speed : velocity;
        return gen().shoot(Bullet::Types[type], {x, y}, velocity, -to_degrees(arctan(velocity)));
    }
};
