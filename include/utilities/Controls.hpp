#pragma once

#include "SFML/Graphics.hpp"
#include "Utils.hpp"

using Joy = sf::Joystick;
using Axis = sf::Joystick::Axis;
using Key = sf::Keyboard;

inline bool pressed_any_of(Key::Key A, Key::Key B) {
    return Key::isKeyPressed(A) ||
           Key::isKeyPressed(B);
}

inline float gamepad_movement(Axis axis, float threshold = 0, unsigned int gamepad_num = 0) {
    float pos = Joy::getAxisPosition(gamepad_num, axis);
    if (std::abs(pos) > threshold) {
        return pos > 0 ? 1 : -1;
    }
    return 0;
}

inline float gamepad_movement(Axis axis1, Axis axis2, float threshold = 0, unsigned int gamepad_num = 0) {
    return sign(gamepad_movement(axis1, threshold, gamepad_num) + gamepad_movement(axis2, threshold, gamepad_num));
}

namespace Gamepad {
    enum Button {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        LB = 4,
        RB = 5,
        SELECT = 6,
        START = 7,
        L3 = 8,
        R3 = 9,
    };
}
