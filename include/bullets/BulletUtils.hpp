#pragma once

#include <functional>

class Bullet;

class UpdateFunc {
private:
    std::function<void(Bullet*, float)> func_;
public:
    UpdateFunc() : func_() {}
    UpdateFunc(const std::function<void(Bullet*, float)>& func) : func_(func) {}

    void operator()(Bullet* bullet, float time) const {
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
    Player
};
