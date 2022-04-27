#pragma once

#include "DynamicObject.hpp"

class Player : public DynamicObject {
  protected:
    float speed_ = 750.f;
  public:
    Player() = default;
    Player(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, 
           sf::Vector2f hitbox_size = {0, 0}, float speed = 750.f, 
           float mass = 0, Layer layer = Layer::Character);
    void on_collide(const DynamicObject* obj) override;
    void control();
};
