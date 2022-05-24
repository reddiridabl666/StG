#pragma once

#include "BulletGenerator.hpp"
#include "Background.hpp"
#include "GameState.hpp"
#include "LoadFiles.hpp"
#include "Player.hpp"
#include "DynamicObject.hpp"
#include "Resources.hpp"
#include "Text.hpp"
#include "UpdateFunctions.hpp"
#include "Wall.hpp"
#include "Window.h"

#include <thread>

class Game {
private:
    Window window;
    Frame frame;
    Background bg;
    // Player* player;

    sf::Clock clock;
    float deltaTime = 0;
public:
    Game() : window(),
             frame(Wall::get_frame(window)),
             bg(Resources::textures["bg"], window) {
             GameState::init(new Player(Resources::textures["player"], 
                             window.getCenter(), {30, 45}, "test_player"), frame);
    }
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void start() { 

        // Wall test({100, 300}, {600, 400});
        // Wall test2({300, 100}, {1200, 700});
        // Wall test3({100, 100}, {400, 900});
        // Wall test4({100, 100}, {1650, 300});

        srand(time(nullptr));
        size_t size = 15;
        float start_pos = 40;
        float delta = window.getView().getSize().x / (size + 1);
        float offset = 0;

        EnemyBulletGenerator test_gen;
        for (size_t i = 0; i < size; ++i) {
            test_gen.shoot(Bullet::BulletTypes["test_circle"], 
                           {start_pos + offset, 150}, 
                           {rand() % 600 - 300.f, rand() % 100 + 25.f},
                           {150, 150});
            offset += delta;
        }

        // test_gen.for_each([] (Bullet* it) {it->scale(2, 2);});
        // test_gen.for_each([] (Bullet* it) {std::cout << "Size: " << it->getFrame()->getSize() << std::endl <<
        //                                              "Half Size: " << it->getFrame()->getHalfSize();});
        // test_gen.for_each([] (Bullet* it) {it->setVelocity(0, 0);});
        // test_gen.for_each([] (Bullet* it) {it->setUpdateFunc(delete_when_out_of_bounds);});

        clock.restart();
        event_loop();
    }

    void event_loop() {
#ifdef DEBUG
        Text timer(std::to_string(GameState::player()->get_invinc_time()),
                   Resources::fonts["ARIAL"], 48, {1650, 450});
#endif
        while (window.isOpen()) {
            deltaTime = clock.restart().asSeconds();
        
            // OS events
            window.sys_event_loop();

            // Character control
            if (window.hasFocus() && GameState::player()) {
                GameState::player()->update();
            }

            // Bullet update
            BulletGenerator::update_all(deltaTime);

            // Movement and collisions
            DynamicObject::move_all(deltaTime);
            
            check_collisions();

            GameState::update();

#ifdef DEBUG
            timer.setString(GameState::player() ? GameState::player()->get_invinc_time() : 0);
#endif

            window.clear(sf::Color::Black);
            GameObject::draw_all(window);

            window.display();

            // if (GameState::player() == nullptr) {
            //     window.close();
            // }
        }
    }

    void check_collisions() {
        if (GameState::player()) {
            DynamicObject::check_collisions_with(*GameState::player());
        }
        for (auto it : frame.iter()) {
            FramedObject::check_phys_collisions_with(*it);
        }
    }

    // ~Game() {if (player) delete player;}
};
