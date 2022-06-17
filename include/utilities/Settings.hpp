#pragma once

#include "Controls.hpp"

#include "nlohmann/json_fwd.hpp"

using json = nlohmann::json;

struct Settings {
private:
    static json settings;
    static sf::Uint16 volume_;

public:
    static void init();

    static void volumeUp(sf::Uint8 vol = 1);
    static void volumeDown(sf::Uint8 vol = 1);

    static sf::Uint16 volume();

    static Key::Key getKey(const std::string& action);
    static Gamepad::Button getButton(const std::string& action);

    static void setKey(const std::string& action, Key::Key k);
    static void setButton(const std::string& action, Gamepad::Button g);

    static void update();
};
