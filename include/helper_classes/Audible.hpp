#pragma once

#include <unordered_map>

#include "SFML/Audio.hpp"

#include "GameState.hpp"
#include "Resources.hpp"

class Audible {
protected:
    const std::unordered_map<std::string, sf::SoundBuffer>& buffers_;
    std::string name_;
    sf::Sound sound_;

public:
    Audible() : buffers_(Resources::sounds) {}

    Audible(const std::unordered_map<std::string, sf::SoundBuffer>& buffers) : buffers_(buffers) {}

    sf::Sound& play_sound(std::string name, float volume_mod = 1) {
        if (name != name_) {
            name_ = name;
            sound_ = sf::Sound(buffers_.at(name));
        }

        sound_.setVolume(GameState::Settings().volume * volume_mod);
        sound_.play();
        return sound_;
    }
};
