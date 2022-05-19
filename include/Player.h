#pragma once

#include "ShootingObject.hpp"
#include "UpdateFunctions.hpp"

class Player : public ShootingObject<BulletGenerator> {
protected:
    float speed_ = 750;
    float normal_speed_ = speed_;
    float slow_speed_ = 320;
    std::string normal_shot_;
    float shot_interval = 0.1;

    int hp_ = 3;
    sf::Clock invincibility_clock_;
    bool is_invincible_ = false;
    float invincibility_time_ = 1.5;

public:
    Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player& other) = delete;

    Player(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
           sf::Vector2f hitbox_size = {0, 0}, const std::string bullet_name = ""/*const BulletInfo& info = {}*/,
           float speed = 750, float mass = 0, Layer layer = Layer::Character);

    void on_collide(DynamicObject* obj) override;
    void on_collide_stop() override;

    void setSpeed(float normal, float slow);

    void update();
    void control();
    void shoot(std::string name) override;

    int gainHP(int n = 1);
    int HP() const;
    int loseHP(int n = 1);
    bool is_invincible() const;
    float get_invinc_time() const;
};
