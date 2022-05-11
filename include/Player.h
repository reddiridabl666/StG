#pragma once

#include "DynamicObject.hpp"
#include "BulletGenerator.hpp"

class Player : public DynamicObject {
  protected:
    float speed_ = 750.f;
    float normal_speed_ = speed_;
    float slow_speed_ = 320.f;
    float shot_interval = 0.1;
    // BulletGenerator gen_;
    sf::Clock clock;

  public:
  BulletGenerator gen_;
    Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player& other) = delete;

    Player(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, 
           sf::Vector2f hitbox_size = {0, 0}, const BulletInfo& info = {}, 
           float speed = 750.f, float mass = 0, Layer layer = Layer::Character);

    void on_collide(DynamicObject* obj) override;
    void setSpeed(float normal, float slow);
    void control();
};
