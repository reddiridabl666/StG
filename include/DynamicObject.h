#pragma once

#include "HitboxObject.hpp"

template <typename HitboxType>
class DynamicObject : public HitboxObject<HitboxType> {
  protected:
    float speed = 0, angle = 0;
  public:
    DynamicObject(const sf::Texture& texture, sf::Vector2f hitbox_size = {0, 0}, Layer layer = Layer::character) 
        : HitboxObject(texture, layer), hitbox_(hitbox_size) {};
};
