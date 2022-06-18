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
        ERR = -1,
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

#include <cassert>

inline std::string to_string(Key::Key key) {
    if (key >= Key::A && key <= Key::Z) {
        return std::string{static_cast<char>(key - Key::A + 'A')};
    }
    static std::array<std::string, Key::KeyCount - Key::Z - 1> keys = {"0", "1", "2", "3", "4", "5", "6", "7", "8",
                                                                    "9", "Escape", "LControl", "LShift",
                                                                    "LAlt", "LSystem", "RControl", "RShift",
                                                                    "RAlt", "RSystem", "Menu", "LBracket",
                                                                    "RBracket", "Semicolon", "Comma", "Period",
                                                                    "Quote", "Slash", "Backslash", "Tilde",
                                                                    "Equal", "Hyphen", "Space", "Enter",
                                                                    "Backspace", "Tab", "PageUp", "PageDown",
                                                                    "End", "Home", "Insert", "Delete",
                                                                    "Add", "Subtract", "Multiply", "Divide",
                                                                    "Left", "Right", "Up", "Down",
                                                                    "Numpad0", "Numpad1", "Numpad2", "Numpad3",
                                                                    "Numpad4", "Numpad5", "Numpad6", "Numpad7",
                                                                    "Numpad8", "Numpad9", "F1", "F2",
                                                                    "F3", "F4", "F5", "F6",
                                                                    "F7", "F8", "F9", "F10",
                                                                    "F11", "F12", "F13", "F14","F15", "Pause"};
    
    return keys[key - Key::Z - 1];
}

inline Key::Key get_key(sf::RenderWindow& window) {
    sf::Clock timeout;

    sf::Event event;
    Key::Key code;

    while (window.waitEvent(event)) {
        if (timeout.getElapsedTime().asSeconds() > 3) {
            return Key::Unknown;
        }
        if (event.type != sf::Event::KeyPressed) {
            continue;
        }
        code = event.key.code;
        break;
    }
    
    return code/*  != Key::Unknown ? code : Key::Num0 */;
}

inline Gamepad::Button get_button(sf::RenderWindow& window) {
    sf::Clock timeout;
    
    sf::Event event;
    Gamepad::Button code;

    while (window.waitEvent(event)) {
        if (timeout.getElapsedTime().asSeconds() > 3) {
            return Gamepad::ERR;
        }
        if (event.type != sf::Event::JoystickButtonPressed) {
            continue;
        }
        code = static_cast<Gamepad::Button>(event.joystickButton.button);
        break;
    }
    
    return code;
}
