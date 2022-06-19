#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void append_file(std::ofstream& out, const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    out << in.rdbuf();
}

void encode(const char* folder_name, const char* result_name) {
    std::ofstream out(result_name, std::ios::binary);

    for (auto& file : fs::directory_iterator(folder_name)) {
        if (file.is_directory()) {
            continue;
        }

        const char* name = file.path().stem().c_str();
        out.write(name, strlen(name) + 1);

        auto size = file.file_size();
        out.write((char*)&size, sizeof(size));

        append_file(out, file.path().c_str());
    }
}

// This can be used to translate all files from the folder to a single binary file
// which can be decoded afterwards with the load_from_file function

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "You should pass name of the folder and name of the resulting file to this program\n";
        return -1;
    }

    encode(argv[1], argv[2]);

    return 0;
}
