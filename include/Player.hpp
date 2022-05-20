#pragma once

#include "ShootingObject.hpp"
#include "UpdateFunctions.hpp"

class Player : public ShootingObject<BulletGenerator>, public Health {
protected:
    float speed_ = 800;
    float normal_speed_ = speed_;
    float slow_speed_ = 450;
    
    std::string normal_shot_;

    sf::Clock invinc_clock_;
    sf::Clock flick_clock_;
    float invinc_time_ = 1.5;

public:
    Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player& other) = delete;

    Player(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
           sf::Vector2f hitbox_size = {0, 0}, const std::string bullet_name = ""/*const BulletInfo& info = {}*/,
           float speed = 800, float mass = 0, Layer layer = Layer::Character);

    void on_collide(DynamicObject* obj) override;
    void on_collide_stop() override;

    void setSpeed(float normal, float slow) {
        normal_speed_ = normal;
        slow_speed_ = slow;
    }

    void update();
    void control();
    void shoot(std::string name) override;

    bool is_invincible() const {
        return !hitbox_->is_active();
    }

    float get_invinc_time() const {
        return is_invincible() ? invinc_time_ - invinc_clock_.getElapsedTime().asSeconds() : 0;
    }
};
