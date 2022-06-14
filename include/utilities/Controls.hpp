#pragma once

#include "SFML/Graphics.hpp"
#include "Utils.hpp"

using Gamepad = sf::Joystick;
using Axis = Gamepad::Axis;
using Key = sf::Keyboard;

inline bool pressed_any_of(Key::Key A, Key::Key B) {
    return Key::isKeyPressed(A) ||
           Key::isKeyPressed(B);
}

inline float gamepad_movement(Axis axis, float threshold = 0, unsigned int gamepad_num = 0) {
    float pos = Gamepad::getAxisPosition(gamepad_num, axis);
    if (std::abs(pos) > threshold) {
        return pos > 0 ? 1 : -1;
    }
    return 0;
}

inline float gamepad_movement(Axis axis1, Axis axis2, float threshold = 0, unsigned int gamepad_num = 0) {
    return sign(gamepad_movement(axis1, threshold, gamepad_num) + gamepad_movement(axis2, threshold, gamepad_num));
}

enum Joy {
    JOY_A = 0,
    JOY_B = 1,
    JOY_X = 2,
    JOY_Y = 3,
    JOY_LB = 4,
    JOY_RB = 5,
    JOY_SEL = 6,
    JOY_ST = 7,
    JOY_L3 = 8,
    JOY_R3 = 9,
};
