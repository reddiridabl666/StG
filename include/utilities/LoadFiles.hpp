#pragma once

#include <array>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

template<typename T>
inline std::unordered_map<std::string, T> load_from_folder(fs::path folder_name){
    std::unordered_map<std::string, T> result;

    for (const auto& file : fs::directory_iterator(folder_name)) {
        if (file.is_directory())
            continue;
        auto filename = file.path().stem().string();
        result[filename] = {};
        result[filename].loadFromFile(file.path().string());
    }
    return result;
}

inline std::vector<sf::Texture> load_row(const sf::Image& image, int num, 
                                         sf::Vector2i pos, sf::Vector2i size = {32, 32}) {
    std::vector<sf::Texture> res(num);
    for (int i = 0; i < num; ++i) {
        res[i].loadFromImage(image, {pos.x + size.x * i, pos.y, size.x, size.y});
    }
    return res;
}

