#pragma once

#include "FramedObject.hpp"
#include "Damage.hpp"

class HealthObject : public FramedObject, public Health {
    using FramedObject::FramedObject;
};
