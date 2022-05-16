#include <SFML/Graphics.hpp>

#include "BulletGenerator.hpp"
#include "Background.hpp"
#include "GameState.hpp"
#include "LoadTextures.hpp"
#include "Player.h"
#include "UpdateFunctions.hpp"
#include "Wall.hpp"
#include "Window.h"

#include <cassert>
#include <cstdlib>
#include <vector>

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");

    // Window initialization
    Window window;

    // Background initialization
    Background bg(textures["bg.jpg"], window);

    // Test walls
    Wall test({100, 300}, {600, 400});
    Wall test2({300, 100}, {1200, 700});
    Wall test3({100, 100}, {400, 900});
    Wall test4({100, 100}, {1650, 300});
    // // Wall test5({800, 800}, CENTER);

    auto frame = Wall::get_frame(window);
    GameState::getState().set_frame(frame);

    auto bullets = Bullet::getBulletTypes();

    // Player initialization
    Player player(textures["player.png"], window.getCenter(), 
                  {30, 45}, bullets["test_player"]);

    // Test circle objs
    srand(time(nullptr));
    size_t size = 50;
    float start_pos = 40;
    float delta = window.getView().getSize().x / (size + 1);
    float offset = 0;

    BulletGenerator test_gen;
    test_gen.add_bullet("circle", bullets["test_circle"]/*{&transp.getTexture(), transp.getSize().x}*/);
    for (size_t i = 0; i < size; ++i) {
        test_gen.shoot("circle");
    }

    test_gen.for_each([&] (Bullet* it) {
        it->setPosition(start_pos + offset, 150);
        it->setVelocity(rand() % 700 - 350.f, rand() % 250 + 350.f);
        offset += delta;
    });

    // Time
    sf::Clock clock;
    float deltaTime;
    
    // Event loop
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
       
        // OS events
        window.sys_event_loop();

        // Character control
        if (window.hasFocus()) {
            player.control();
        }

        // Bullet update
        BulletGenerator::update_all();

        // Movement
        DynamicObject::move_all(deltaTime);

        GameState::getState().update(player.getPosition());

        // Collision checks
        // DynamicObject::check_collisions_with(player);
        DynamicObject::check_collisions();

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here
        GameObject::draw_all(window);

        // end the current frame
        window.display();
    }
    return 0;
}
