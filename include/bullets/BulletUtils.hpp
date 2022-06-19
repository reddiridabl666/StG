#pragma once

#include <functional>

#include "SFML/System.hpp"

class Bullet;

void delete_when_out_of_bounds(Bullet*, float);
void delete_if_near_player(Bullet* bullet, float);

namespace constants {
    inline constexpr float g = 9.8 * 100;
}

class UpdateFunc {
private:
    std::function<void(Bullet*, float)> func_ = [] (Bullet*, float) {};
public:
    UpdateFunc() = default;
    UpdateFunc(const std::function<void(Bullet*, float)>& func) : func_(func) {}

    void operator()(Bullet* bullet, float time) const {
        delete_when_out_of_bounds(bullet, time);
        delete_if_near_player(bullet, time);
        func_(bullet, time);
    }
    
    UpdateFunc operator+(const UpdateFunc& other) const {
        return UpdateFunc([this, other](Bullet* bullet, float time) {
            (*this)(bullet, time);
            other(bullet, time);
        });
    }
};

const UpdateFunc delete_timed(float time_in_seconds);
const UpdateFunc gravity();
const UpdateFunc circular(sf::Vector2f center, float speed);

enum class BulletType {
    BigCircle_Red,
    Circle_Red,
    Talisman_RB,
    Seed_Red,
    Player
};
