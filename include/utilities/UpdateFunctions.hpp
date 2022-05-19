#pragma once

#include <functional>

#include "Bullet.hpp"
#include "GameState.hpp"

inline const UpdateFunc delete_when_out_of_bounds = [] (Bullet* bullet, float) {
    if (GameState::getState().is_out_of_bounds(bullet)) {
        bullet->deactivate();
    }
};

template<int time_in_milliseconds>
inline const UpdateFunc delete_timed = [] (Bullet* bullet, float) {
    if (bullet->getTime().asMilliseconds() > time_in_milliseconds) {
        bullet->deactivate();
    }
};

inline const UpdateFunc gravity = [] (Bullet* bullet, float deltaTime) {
    // auto eps = 5;
    auto y = bullet->getVelocity().y + constants::g * deltaTime * 80;
    auto x = bullet->getVelocity().x;
    bullet->setVelocity(x, y);
};
