#pragma once

#include <functional>

#include "Bullet.hpp"
#include "GameState.hpp"

inline const std::function<void(Bullet* it)> delete_when_out_of_bounds = [] (Bullet* bullet) {
    if (GameState::getState().is_out_of_bounds(bullet)) {
        bullet->deactivate();
    }
};
