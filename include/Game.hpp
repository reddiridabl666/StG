#pragma once

#include "BulletGenerator.hpp"
#include "Background.hpp"
#include "GameState.hpp"
#include "LoadFiles.hpp"
#include "Player.h"
#include "Text.hpp"
#include "UpdateFunctions.hpp"
#include "Wall.hpp"
#include "Window.h"

class Game {
private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    Window window;
    Background bg;
    Player* player;

    sf::Clock clock;
    float deltaTime = 0;
public:
    Game() : textures(load_from_folder<sf::Texture>("images")),
             fonts(load_from_folder<sf::Font>("fonts")),
             window(),
             bg(textures["bg"], window),
             player(new Player(textures["player"], window.getCenter(), 
                    {30, 45}, "test_player")) {}
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start() {
        auto frame = Wall::get_frame(window);
        GameState::getState().set_frame(frame); 

        Wall test({100, 300}, {600, 400});
        Wall test2({300, 100}, {1200, 700});
        Wall test3({100, 100}, {400, 900});
        Wall test4({100, 100}, {1650, 300});

        srand(time(nullptr));
        size_t size = 25;
        float start_pos = 40;
        float delta = window.getView().getSize().x / (size + 1);
        float offset = 0;

        EnemyBulletGenerator test_gen;
        for (size_t i = 0; i < size; ++i) {
            test_gen.shoot(Bullet::BulletTypes["test_circle"], 
                           {start_pos + offset, 150}, 
                           {rand() % 300 - 150.f, rand() % 50 + 25.f});
            offset += delta;
        }

        clock.restart();
        event_loop();
    }

    // TODO: мб стоит передавать сюда функцию?
    void event_loop() {
        Text hp(std::to_string(player->HP()), 
                fonts["ARIAL"], 48, {1650, 150});

        Text timer(std::to_string(player->get_invinc_time()),
                   fonts["ARIAL"], 48, {1650, 450});

        while (window.isOpen()) {
            deltaTime = clock.restart().asSeconds();
        
            // OS events
            window.sys_event_loop();

            // Character control
            if (window.hasFocus() && player) {
                player->update();
            }

            // Bullet update
            BulletGenerator::update_all(deltaTime);

            // Movement
            DynamicObject::move_all(deltaTime);
            
            // Collision checks
            // DynamicObject::check_collisions_with(player);
            DynamicObject::check_collisions();

            // TODO: В отделную функцию?
            if (player && player->HP() <= 0) {
                delete player;
                player = nullptr;
            }

            // TODO: Вынести в отдельную функцию
            hp.setString(player ? std::string("Health: ") + std::to_string(player->HP()) : "You died");
            timer.setString(player ? player->get_invinc_time() : 0);

            window.clear(sf::Color::Black);
            GameObject::draw_all(window);

            window.display();
        }
    }

    ~Game() {if (player) delete player;}
};
