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
    class Manager {
        std::list<std::shared_ptr<GameObject>> objs;
    public:
        Manager() = default;
        void update() {
            for (auto it = objs.begin(); it != objs.end();) {
                if (!(*it)->is_active()) {
                    objs.erase(it);
                } else {
                    ++it;
                }
            }
        }

        template <typename T>
        std::weak_ptr<T> add(T* obj) {
            objs.emplace_back(obj);
            return std::shared_ptr<T>(obj);
        }
    };
    
    Manager manager;
    Window window;
    Frame frame;
    Background bg;
    
    std::weak_ptr<Player> player;
    std::weak_ptr<Boss> boss;

    sf::Clock clock;
    float deltaTime = 0;
public:
    Game() : manager(), 
             window(),
             frame(Wall::get_frame(window)),
             bg(Resources::textures["bg"], window),
             player(manager.add(new Player(window.getCenter(), {25, 30}))) {
             GameState::init(player.lock().get(), &window, frame);
    }
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start();

    void event_loop();

    void check_collisions();

    // ~Game() {
    //     if (player) {
    //         delete player; 
    //         player = nullptr;
    //     }
    //     if (boss) {
    //         delete boss; 
    //         boss = nullptr;
    //     }
    // }
};
