#pragma once

#include "BulletGenerator.hpp"
#include "Background.hpp"
#include "GameState.hpp"
#include "LoadFiles.hpp"
#include "Player.hpp"
#include "Resources.hpp"
#include "Text.hpp"
#include "UpdateFunctions.hpp"
#include "Wall.hpp"
#include "Window.h"

class Game {
private:
    // string_map<sf::Texture> textures;
    // string_map<sf::Font> fonts;
    // string_map<sf::Image> sprite_sheets;
    // Resources resources;

    Window window;
    Frame frame;
    Background bg;
    Player* player;

    sf::Clock clock;
    float deltaTime = 0;
public:
    Game() :  /* textures(load_from_folder<sf::Texture>("images")), */
    //          fonts(load_from_folder<sf::Font>("fonts")),
    //          sprite_sheets(load_from_folder<sf::Image>("images/sprite_sheets")),
             window(),
             frame(Wall::get_frame(window)),
             bg(Resources::textures["bg"], window),
             player(new Player(Resources::textures["player"], 
                    window.getCenter(), 
                    {30, 45}, "test_player")) {}
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start() {
        // auto frame = Wall::get_frame(window);
        GameState::set_frame(frame); 

        // Wall test({100, 300}, {600, 400});
        // Wall test2({300, 100}, {1200, 700});
        // Wall test3({100, 100}, {400, 900});
        // Wall test4({100, 100}, {1650, 300});

        srand(time(nullptr));
        size_t size = 25;
        float start_pos = 40;
        float delta = window.getView().getSize().x / (size + 1);
        float offset = 0;

        EnemyBulletGenerator test_gen;
        for (size_t i = 0; i < size; ++i) {
            test_gen.shoot(Bullet::BulletTypes["test_circle"], 
                           {start_pos + offset, 150}, 
                           {rand() % 600 - 300.f, rand() % 50 + 25.f},
                           {100, 100});
            offset += delta;
        }

        // test_gen.for_each([] (Bullet* it) {it->scale(4, 4);});
        // test_gen.for_each([] (Bullet* it) {it->setVelocity(0, 0);});
        // test_gen.for_each([] (Bullet* it) {it->setUpdateFunc(delete_when_out_of_bounds);});

        clock.restart();
        event_loop();
    }

    void event_loop() {
        Text hp(std::to_string(player->HP()), 
                Resources::fonts["ARIAL"], 48, {1650, 150});
#ifdef DEBUG
        Text timer(std::to_string(player->get_invinc_time()),
                   Resources::fonts["ARIAL"], 48, {1650, 450});
#endif

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
            check_collisions();
            // DynamicObject::check_collisions();

            // TODO: В отдельную функцию?
            if (player && player->HP() <= 0) {
                delete player;
                player = nullptr;
            }

            // TODO: Вынести в отдельную функцию
            hp.setString(player ? std::string("Health: ") + std::to_string(player->HP()) : "You died");
#ifdef DEBUG
            timer.setString(player ? player->get_invinc_time() : 0);
#endif

            window.clear(sf::Color::Black);
            GameObject::draw_all(window);

            window.display();
        }
    }

    void check_collisions() {
        if (player) {
            DynamicObject::check_collisions_with(*player);
        }
        for (auto it : frame.iter()) {
            DynamicObject::check_collisions_with(*it);
        }
    }

    ~Game() {if (player) delete player;}
};
