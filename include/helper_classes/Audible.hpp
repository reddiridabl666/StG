#pragma once

#include <unordered_map>

#include "SFML/Audio.hpp"

#include "Resources.hpp"

class Audible {
protected:
    const std::unordered_map<std::string, sf::SoundBuffer>& buffers_;
    std::string name_;
    sf::Sound sound_;

public:
    Audible() : buffers_(Resources::sounds) {}

    Audible(const std::unordered_map<std::string, sf::SoundBuffer>& buffers) : buffers_(buffers) {}

    sf::Sound& play_sound(std::string name, float volume = 100) {
        if (name != name_) {
            name_ = name;
            sound_ = sf::Sound(buffers_.at(name));
        }
        // if (sound_.getStatus() != sf::SoundSource::Status::Playing)
        sound_.setVolume(volume);
        sound_.play();
        return sound_;
    }
};
