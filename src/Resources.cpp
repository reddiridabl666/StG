#include "LoadFiles.hpp"
#include "Resources.hpp"
#include "Text.hpp"
#include "Bullet.hpp"

string_map<sf::Texture> Resources::textures = load_from_folder<sf::Texture>("images");
string_map<sf::Font> Resources::fonts = load_from_folder<sf::Font>("fonts");
string_map<sf::Image> Resources::sprite_sheets = load_from_folder<sf::Image>("images/sprite_sheets");
string_map<sf::SoundBuffer> Resources::sounds = load_from_folder<sf::SoundBuffer>("sounds");

const sf::Font& Text::DefaultFont = Resources::fonts["Nova+"];

std::unordered_map<BulletType, Bullet::Info> Bullet::Types = Bullet::getBulletTypes();
