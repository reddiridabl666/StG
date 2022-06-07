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

        if (phase_num_ > phase_max_) {
            deactivate();
        }
    }

    friend class Phase;
};

class AnimatedBoss : public Boss, public Animated {
public:
    using Boss::Boss;

    void setTexture(const sf::Texture& texture) override {
        Boss::setTexture(texture);
    }

    void update(float deltaTime) override {
        Boss::update(deltaTime);
        Animated::update();
    }
};

struct Phase {
protected:
    Boss* parent;
public:
    explicit Phase(Boss* parent, int hp = 7000) : parent(parent) {
        parent->setHP(hp);
    }

    int hp() {
        return parent->HP();
    }

    auto& gen() {
        return parent->gen_;
    }

    int& shot_num() {
        return parent->shot_num_;
    }
    sf::Clock& shoot_clock() {
        return parent->shoot_clock_;
    }

    virtual void update(float) = 0;
    virtual void shoot() = 0;
};
