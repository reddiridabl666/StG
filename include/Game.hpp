#pragma once

#include "Background.hpp"
#include "TestBoss.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include "Resources.hpp"
#include "Settings.hpp"
#include "Text.hpp"
#include "Wall.hpp"
#include "Window.h"
#include "Button.hpp"

class Menu;

class Game {
private:
    class Manager {
        std::list<std::shared_ptr<GameObject>> objs;
    public:
        Manager() = default;
        
        void update();

        void refresh();

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
    bool paused = false;
    bool in_main_menu = false;
    bool in_game = false;
    bool back_pressed = false;

    sf::Clock clock;
    float deltaTime = 0;

private:
    void menu(sf::String text, const std::vector<Button::Info>& infos, const std::function<void()>& func = [] {});
    // void settings_menu(sf::String text, const std::vector<Button::Info>& infos);

    void main_menu();

    // template <typename T>
    void controls_menu(const std::string& label, const std::vector<std::string>& actions, bool gamepad = false);

    std::vector<Button::Info> getControls(std::map<std::string, CenteredText>& keys, bool gamepad, sf::Clock& timer);
    void pause_menu();

    void settings();
    void volume();
    void controls();

    void event_loop(const std::function<void()>& action = [] {});

    void game_loop();

    void game_over();

    void check_collisions();

    Background gray_if_paused();

public:
    Game() : 
        manager(), 
        window(*this),
        frame(Wall::get_frame(window)),
        bg(Resources::textures["bg"], window) {
        GameState::init(&window, frame);
        Settings::init();
    }
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start();

    friend Window;
};
