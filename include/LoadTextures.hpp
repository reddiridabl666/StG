#pragma once

#include <array>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

static constexpr uint16_t texture_num = 20;

namespace fs = std::filesystem;

inline std::unordered_map<std::string, sf::Texture> load_textures(fs::path folder_name){
    std::unordered_map<std::string, sf::Texture> result;

    for (const auto& file : fs::directory_iterator(folder_name)) {
        auto filename = file.path().string();
        result[filename] = {};
        result[filename].loadFromFile(filename);
    }

    return result;
}
