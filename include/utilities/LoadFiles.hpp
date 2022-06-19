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

static std::string read_name(std::ifstream& in) {
    std::cout << "Reading filename...\n";

    std::string res;
    int c;
    while ((c = in.get()) != '\0' && c != in.eof()) {
        res += (char)c;
    }
    return res;
}

template<typename T>
inline std::unordered_map<std::string, T> load_from_file(fs::path file_name){
    std::unordered_map<std::string, T> result;

    std::ifstream in(file_name, std::ios::binary);

    std::string name;
    uintmax_t size;
    std::vector<char> buff;

    while (!in.eof()) {
        name = read_name(in);
        std::cout<< name << " was read!\n";

        std::cout << "Reading filesize...\n";
        in.read((char*)&size, sizeof(size));
        buff.resize(size);

        std::cout << "Reading file...\n";
        in.read(buff.data(), size);
        std::cout << "Loading file...\n\n";
        result[name].loadFromMemory(buff.data(), size);
    }

    return result;
}
