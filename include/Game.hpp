#pragma once

#include "BulletGenerator.hpp"
#include "Background.hpp"
#include "GameState.hpp"
#include "LoadTextures.hpp"
#include "Player.h"
#include "UpdateFunctions.hpp"
#include "Wall.hpp"
#include "Window.h"

class Game {
using objPtr = std::unique_ptr<GameObjectBase>;
private:
    Player player;
    Window window;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, BulletInfo> bullets;
    // std::list<
public:
    Game();
    ~Game();
};
