#pragma once

#include <array>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

static constexpr sf::Uint16 texture_num = 20;  // ???

namespace fs = std::filesystem;

template<typename T>
inline std::unordered_map<std::string, T> load_from_folder(fs::path folder_name){
    std::unordered_map<std::string, T> result;

    for (const auto& file : fs::directory_iterator(folder_name)) {
        auto filename = file.path().stem().string();
        result[filename] = {};
        result[filename].loadFromFile(file.path().string());
    }
    return result;
}
