#pragma once

#include <unordered_set>
#include "GameObject.h"

class Ui : GameObject {
private:
    static std::unordered_set<Ui*> all;
public:

};
