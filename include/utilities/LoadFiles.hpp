#pragma once

#include <array>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

#ifndef RELEASE
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
#else
static std::string read_name(std::ifstream& in) {
    std::string res;
    int c;
    while ((c = in.get()) != '\0') {
        if (in.eof()) {
            throw std::runtime_error("End of file");
        }
        res += (char)c;
    }
    return res;
}

template <typename T>
void load(T& asset, const char* buff, size_t size) {
    asset.loadFromMemory(buff, size);
}

template <>
void load(sf::Font& asset, const char* buff, size_t size) {
    static std::vector<std::string> static_buff;
    static_buff.emplace_back(buff, size);
    asset.loadFromMemory(static_buff.back().c_str(), size);
}

template<typename T>
inline std::unordered_map<std::string, T> load_from_file(fs::path file_name){
    std::unordered_map<std::string, T> result;

    std::ifstream in(file_name, std::ios::binary);

    if (!in) {
        throw std::runtime_error("Missing assets");
    }

    std::string name;
    uintmax_t size;
    std::vector<char> buff;

    while (!in.eof()) {
        try {
            name = read_name(in);
        }
        catch(std::runtime_error& e) {
            break;
        }
        
        in.read((char*)&size, sizeof(size));
        buff.resize(size);

        in.read(buff.data(), size);
        load<T>(result[name], buff.data(), size);
    }

    return result;
}
#endif