#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

template<typename T>
using string_map = std::unordered_map<std::string, T>;

struct Resources {
public:
    static string_map<sf::Texture> textures;
    static string_map<sf::Font> fonts;
    static string_map<sf::Image> sprite_sheets;
    static string_map<sf::SoundBuffer> sounds;

    Resources() = delete;
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
};
