#include "Settings.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

json Settings::settings;
sf::Uint16 Settings::volume_;

void Settings::init() {
    std::ifstream in("settings.json");

    in >> settings;

    volume_ = settings["volume"];
}

void Settings::volumeUp(sf::Uint8 vol) {
    volume_ = volume_ + vol > 100 ? 100 : volume_ + vol;
    settings["volume"] = volume_;
}

void Settings::volumeDown(sf::Uint8 vol) {
    volume_ = volume_ - vol < 0 ? 0 : volume_ - vol;
    settings["volume"] = volume_;
}

sf::Uint16 Settings::volume() {
    return volume_;
}

Key::Key Settings::getKey(const std::string& action) {
    return settings[action];
}

Gamepad::Button Settings::getButton(const std::string& action) {
    return static_cast<Gamepad::Button>(getKey(action));
}

void Settings::setKey(const std::string& action, Key::Key k) {
    settings[action] = k;
}

void Settings::setButton(const std::string& action, Gamepad::Button g) {
    setKey(action, static_cast<Key::Key>(g));
}

void Settings::update() {
    std::ofstream out("settings.json");
    out << settings.dump(4);
}
