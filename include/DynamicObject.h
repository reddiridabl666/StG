#pragma once

#include "HitboxObject.hpp"

template <typename HitboxType>
class DynamicObject : public HitboxObject<HitboxType> {
  protected:
    float mass, speed;
  public:
    DynamicObject();
};
