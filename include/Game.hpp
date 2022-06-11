#pragma once

#include "Background.hpp"
#include "Boss.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include "Resources.hpp"
#include "Text.hpp"
#include "Wall.hpp"
#include "Window.h"

#include <thread>

class Game {
private:
    Window window;
    Frame frame;
    Background bg;
    Player* player;

    Boss* boss;

    sf::Clock clock;
    float deltaTime = 0;
public:
    Game() : window(),
             frame(Wall::get_frame(window)),
             bg(Resources::textures["bg"], window),
             player(new Player(window.getCenter(), {25, 30})) {
             GameState::init(player, &window, frame);
    }
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start();

    void event_loop();

    void check_collisions();

    ~Game() {
        if (player) {
            delete player; 
            player = nullptr;
        }
        if (boss) {
            delete boss; 
            boss = nullptr;
        }
    }
};
