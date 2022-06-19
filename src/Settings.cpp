#include "Settings.hpp"
#include "GameState.hpp"
#include "Audible.hpp"
#include "Player.hpp"

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

void fix(const json::exception& e) {
    Settings::rebuild_json();
    error_prompt(e);
}

void Settings::rebuild_json() {
    settings = R"({
                    "Keyboard": {
                        "shoot": 57,
                        "slow": 38
                    },
                    "Gamepad": {
                        "shoot": 0,
                        "slow": 5
                    },
                    "volume": 100
                })"_json;
}

void Settings::init() {
    std::ifstream in("settings.json");
    
    try {
        if (!in) {
            rebuild_json();
            in.close();
            update();
        } else {
            in >> settings;
        }
    }
    catch (const json::exception& e) {
        fix(e);
    }

    Player::setShoot(getKey("shoot"), getButton("shoot"));
    Player::setSlow(getKey("slow"), getButton("slow"));
        
    try {
        volume_ = settings["volume"];
    }
    catch (const json::exception& e) {
        fix(e);
        volume_ = settings["volume"];
    }

    Audible::setVolume(volume_);
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

int Settings::get(const std::string& what, const std::string& action) {
    try {
        return settings[what][action];
    } 
    catch (const json::exception& e) {
        fix(e);
        return get(what, action);
    }
}

Key::Key Settings::getKey(const std::string& action) {
    return static_cast<Key::Key>(get("Keyboard", action));
}

Gamepad::Button Settings::getButton(const std::string& action) {
    return static_cast<Gamepad::Button>(get("Gamepad", action));
}

void Settings::setKey(const std::string& action, Key::Key k) {
    set("Keyboard", action, k);
}

void Settings::setButton(const std::string& action, Gamepad::Button g) {
    set("Gamepad", action, g);
}

void Settings::set(const std::string& what, const std::string& action, int k) {
    settings[what][action] = k;
}

void Settings::update() {
    std::ofstream out("settings.json");
    out << settings.dump(4);

    Audible::setVolume(volume_);
    Player::setShoot(getKey("shoot"), getButton("shoot"));
    Player::setSlow(getKey("slow"), getButton("slow"));
}
