#include "Settings.hpp"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

json Settings::settings;
sf::Uint16 Settings::volume_;

static void error_prompt(const json::exception& e) {
    std::ofstream err("error.log");
    err << "Something bad happened with your settings file!" << std::endl;
    err << e.what() << std::endl;
    err << "Settings have been reset!"<< std::endl;
}

void Settings::rebuild_json() {
    settings = R"({
                    "key": {
                        "shoot": 57,
                        "slow": 38
                    },
                    "joy": {
                        "shoot": 0,
                        "slow": 5
                    },
                    "volume": 100
                })"_json;
}

void Settings::init() {
    std::ifstream in("settings.json");
    try {
        in >> settings;
        volume_ = settings["volume"];
    }
    catch(const json::exception& e) {
        rebuild_json();
        error_prompt(e);
        volume_ = settings["volume"];
    }
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
    try {
        return settings["key"][action];
    }
    catch(const json::exception& e) {
        rebuild_json();
        error_prompt(e);
        return getKey(action);
    }
}

Gamepad::Button Settings::getButton(const std::string& action) {
    try {
        return settings["joy"][action];
    } 
    catch (const json::exception& e) {
        rebuild_json();
        error_prompt(e);
        return getButton(action);
    }
}

void Settings::setKey(const std::string& action, Key::Key k) {
    settings["key"][action] = k;
}

void Settings::setButton(const std::string& action, Gamepad::Button g) {
    settings["joy"][action] = g;
}

void Settings::update() {
    std::ofstream out("settings.json");
    out << settings.dump(4);
}
