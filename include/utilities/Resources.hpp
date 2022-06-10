#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <filesystem>

#include "LoadFiles.hpp"

template<typename T>
using string_map = std::unordered_map<std::string, T>;
namespace fs = std::filesystem;

struct Resources {
public:
    static inline string_map<sf::Texture> textures;
    static inline string_map<sf::Font> fonts;
    static inline string_map<sf::Image> sprite_sheets;
    static inline string_map<sf::SoundBuffer> sounds;

    Resources(fs::path texture_path, fs::path fonts_path, fs::path sheets_path, fs::path sounds_path) {
        textures = load_from_folder<sf::Texture>(texture_path);
        fonts = load_from_folder<sf::Font>(fonts_path);
        sprite_sheets = load_from_folder<sf::Image>(sheets_path);
        sounds = load_from_folder<sf::SoundBuffer>(sounds_path);
    }
private:
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
};

const inline Resources resources("images", "fonts", "images/sprite_sheets", "sounds");
