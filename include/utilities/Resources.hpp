#pragma once

#include "SFML/Graphics.hpp"
#include <filesystem>
#include "LoadFiles.hpp"

template<typename T>
using string_map = std::unordered_map<std::string, T>;
namespace fs = std::filesystem;

struct Resources {
public:
    static string_map<sf::Texture> textures;
    static string_map<sf::Font> fonts;
    static string_map<sf::Image> sprite_sheets;

    Resources(fs::path texture_path, fs::path fonts_path, fs::path sheets_path) {
        textures = load_from_folder<sf::Texture>(texture_path);
        fonts = load_from_folder<sf::Font>(fonts_path);
        sprite_sheets = load_from_folder<sf::Image>(sheets_path);
    }
private:
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
};


inline string_map<sf::Texture> Resources::textures;
inline string_map<sf::Font> Resources::fonts;
inline string_map<sf::Image> Resources::sprite_sheets;

static inline Resources resources("images", "fonts", "images/sprite_sheets");
