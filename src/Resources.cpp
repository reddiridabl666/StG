#include "LoadFiles.hpp"
#include "Resources.hpp"
#include "Text.hpp"
#include "Bullet.hpp"

#ifdef RELEASE
string_map<sf::Texture> Resources::textures = load_from_file<sf::Texture>("data/data1.dat");
string_map<sf::Font> Resources::fonts = load_from_file<sf::Font>("data/data4.dat");
string_map<sf::Image> Resources::sprite_sheets = load_from_file<sf::Image>("data/data2.dat");
string_map<sf::SoundBuffer> Resources::sounds = load_from_file<sf::SoundBuffer>("data/data3.dat");
#else
string_map<sf::Texture> Resources::textures = load_from_folder<sf::Texture>("images");
string_map<sf::Font> Resources::fonts = load_from_folder<sf::Font>("fonts");
string_map<sf::Image> Resources::sprite_sheets = load_from_folder<sf::Image>("images/sprite_sheets");
string_map<sf::SoundBuffer> Resources::sounds = load_from_folder<sf::SoundBuffer>("sounds");
#endif

const sf::Font& Text::DefaultFont = Resources::fonts["Nova+"];

std::unordered_map<BulletType, Bullet::Info> Bullet::Types = Bullet::getBulletTypes();
