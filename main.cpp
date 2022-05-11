#include <SFML/Graphics.hpp>

#include "BulletGenerator.hpp"
#include "GameObject.h"
#include "DynamicObject.hpp"
#include "Wall.hpp"
#include "LoadTextures.hpp"
#include "Player.h"
#include "Window.h"

#include <vector>
#include <cassert>
#include <cstdlib>

#define CENTER static_cast<sf::Vector2f>(window.getSize()) / 2.f

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");

    // Init window
    Window window;
    auto win_size = window.getView().getSize();

    // Background initialization
    GameObject bg(textures["bg.jpg"], CENTER);
    auto factor = std::max(static_cast<float>(window.getSize().x) / bg.getSize().x,
                           static_cast<float>(window.getSize().y) / bg.getSize().y);
    bg.scale(factor, factor);

    // Test walls
    Wall test({100, 300}, {600, 400});
    Wall test2({300, 100}, {1200, 700});
    Wall test3({100, 100}, {400, 900});
    Wall test4({100, 100}, {1650, 300});
    // Wall test5({800, 800}, CENTER);

    // Frame walls
    float bound_width = 300;
    Wall left_bound({bound_width, win_size.y}, {-bound_width / 2, win_size.y / 2});
    Wall right_bound({bound_width, win_size.y}, {win_size.x + bound_width / 2, win_size.y / 2});
    Wall up_bound({win_size.x, bound_width}, {win_size.x / 2, -bound_width / 2});
    Wall low_bound({win_size.x, bound_width}, {win_size.x / 2, win_size.y + bound_width / 2});

    std::function<void(Bullet* it)> delete_when_out_of_bounds = [&] (Bullet* bullet) {
        float offset = 100;
        if (bullet->getPosition().x > right_bound.getPosition().x + offset ||
            bullet->getPosition().x < left_bound.getPosition().x - offset ||
            bullet->getPosition().y > low_bound.getPosition().y + offset ||
            bullet->getPosition().y < up_bound.getPosition().y - offset) {
                bullet->deactivate();
        }
        // std::cout << "right: " << right_bound.getPosition().x + offset << " left: " << left_bound.getPosition().x - offset 
        //     << " up: " << up_bound.getPosition().y - offset << " low: " << low_bound.getPosition().y + offset << std::endl;
        // std::cout << bullet->getPosition().x << ", " << bullet->getPosition().y << std::endl;
    };

    // Player initialization
    sf::RenderTexture bullet;
    bullet.create(20, 40);
    bullet.clear(sf::Color::Transparent);
    BulletInfo info = {&bullet.getTexture(), static_cast<sf::Vector2f>(bullet.getSize()), 
                        {0, -600}, &delete_when_out_of_bounds};
    Player player(textures["player.png"], CENTER, {30, 45}, info);
    player.scale(4.4f, 4.4f);

    // Test circle objs
    sf::RenderTexture transp;
    transp.create(30, 30);
    sf::CircleShape circle(transp.getSize().x);
    circle.setFillColor(sf::Color::Transparent);
    transp.draw(circle);

    srand(time(nullptr));
    size_t size = 50;
    float start_pos = 40;
    float delta = window.getView().getSize().x / (size + 1);
    float offset = 0;

    BulletGenerator test_gen;
    test_gen.add_bullet("circle", {&transp.getTexture(), transp.getSize().x});
    for (size_t i = 0; i < size; ++i) {
        test_gen.shoot("circle");
    }

    test_gen.for_each([&] (Bullet* it) {
        it->setMass(1);
        it->setPosition(start_pos + offset, 150);
        it->setVelocity(rand() % 700 - 350.f, rand() % 250 + 350.f);
        it->setUpdateFunc(delete_when_out_of_bounds);
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
