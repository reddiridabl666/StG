#pragma once

#include "HitboxObject.hpp"

class Player : public HitboxObject<RectHitbox> {
  protected:  
    float speed_ = 750.f;
  public:
    Player() = default;
    Player(const sf::Texture& texture, sf::Vector2f hitbox_size = {0, 0}, 
           Layer layer = Layer::character, float speed = 750.f);
    void control(float deltaTime);
};
