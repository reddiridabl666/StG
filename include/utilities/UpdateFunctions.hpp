#pragma once

#include <functional>

#include "Bullet.hpp"
#include "GameState.hpp"
#include "Math.hpp"

namespace constants {
    inline constexpr float g = 9.8 * 80;
}

inline const UpdateFunc delete_when_out_of_bounds([] (Bullet* bullet, float) {
    if (GameState::is_out_of_bounds(bullet)) {
        bullet->deactivate();
    }
});

inline const UpdateFunc delete_timed(float time_in_seconds) {
    return std::function<void(Bullet*, float)>([time_in_seconds] (Bullet* bullet, float) {
        if (bullet->getTime().asMilliseconds() > time_in_seconds) {
            bullet->deactivate();
        }
    });
}

// inline const UpdateFunc delete_timed([] (Bullet* bullet, float) {
//     if (bullet->getTime().asMilliseconds() > time_in_seconds) {
//         bullet->deactivate();
//     }
// });

inline const UpdateFunc gravity([] (Bullet* bullet, float deltaTime) {
    // auto eps = 5;
    auto y = bullet->getVelocity().y + constants::g * deltaTime;
    auto x = bullet->getVelocity().x;
    bullet->setVelocity(x, y);
});

inline const UpdateFunc circular(sf::Vector2f center, float speed) {
    return std::function<void(Bullet*, float)>([center, speed] (Bullet* bullet, float deltaTime) {
        auto a = unit_vector(center, bullet->getPosition()) * deltaTime * speed;
        a = {a.y, -a.x};
        bullet->setVelocity(bullet->getVelocity() + a);
    });
}

