#pragma once

#include "Background.hpp"
#include "TestBoss.hpp"
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
                    it = objs.erase(it);
                } else {
                    ++it;
                }
            }
        }

        template <typename T>
        std::shared_ptr<T> add(std::shared_ptr<T>&& obj) {
            objs.push_back(obj);
            return obj;
        }
    };
    
    Manager manager;
    Window window;
    Frame frame;
    Background bg;
    
    std::weak_ptr<Player> player;
    std::weak_ptr<Boss> boss;

    bool in_loop = false;

    sf::Clock clock;
    float deltaTime = 0;

protected:
    void main_menu();

    void pause_menu();

    void event_loop(const std::function<void()>& action = [] {});

    void game_loop();

    void check_collisions();

public:
    Game() : manager(), 
             window(),
             frame(Wall::get_frame(window)),
             bg(Resources::textures["bg"], window),
             player(manager.add(std::make_shared<Player>(window.getCenter(), sf::Vector2f{25, 30}))),
             boss(manager.add(std::make_shared<TestBoss>(sf::Vector2f{window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f}))) {
             GameState::init(player, &window, frame);
    }
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start();
};
