#pragma once

#include "GameObject.h"

class Hitbox : public sf::Transformable {
  public:
    virtual bool collides_with(sf::Transformable target) = 0;
};

class RectHitbox : Hitbox {
  private:
    sf::Uint32 width_, height_;
    
};

class HitboxObject : public GameObject {
  protected:
    Hitbox *hitbox_;

  public:
    HitboxObject();
    HitboxObject(sf::Texture* texture, sf::Uint8 layer = 0, Hitbox *hitbox = {});
};
