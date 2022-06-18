#pragma once

#include <unordered_map>

#include "SFML/Audio.hpp"

#include "Settings.hpp"
#include "Resources.hpp"

class Audible {
protected:
    const std::unordered_map<std::string, sf::SoundBuffer>& buffers_;
    std::string name_;
    sf::Sound sound_;
    static inline sf::Uint16 volume_;

public:
    Audible() : buffers_(Resources::sounds) {}

    static void setVolume(sf::Uint16 volume) {
        volume_ = volume;
    }
    
    static sf::Uint16 getVolume() {
        return volume_;
    }

    Audible(const std::unordered_map<std::string, sf::SoundBuffer>& buffers) : buffers_(buffers) {}

    sf::Sound& play_sound(std::string name, float volume_mod = 1) {
        if (name != name_) {
            name_ = name;
            sound_ = sf::Sound(buffers_.at(name));
        }

        sound_.setVolume(volume_ * volume_mod);
        sound_.play();
        return sound_;
    }
};
