#pragma once

#include <functional>

class Bullet;

void delete_when_out_of_bounds(Bullet*, float);

class UpdateFunc {
private:
    std::function<void(Bullet*, float)> func_ = [] (Bullet*, float) {};
public:
    UpdateFunc() = default;
    UpdateFunc(const std::function<void(Bullet*, float)>& func) : func_(func) {}

    void operator()(Bullet* bullet, float time) const {
        delete_when_out_of_bounds(bullet, time);
        func_(bullet, time);
    }
    
    UpdateFunc operator+(const UpdateFunc& other) const {
        return UpdateFunc([this, other](Bullet* bullet, float time) {
            (*this)(bullet, time);
            other(bullet, time);
        });
    }
};

enum class BulletType {
    BigCircle_Red,
    Circle_Red,
    Talisman_RB,
    Seed_Red,
    Player
};
