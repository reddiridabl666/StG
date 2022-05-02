#include <SFML/Graphics.hpp>

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

// Init window
Window window;
auto win_size = window.getView().getSize();

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");

    // Background initialization
    GameObject bg(textures["bg.jpg"], CENTER);
    auto factor = std::max(static_cast<float>(window.getSize().x) / bg.getSize().x,
                           static_cast<float>(window.getSize().y) / bg.getSize().y);
    bg.scale(factor, factor);

    // Player initialization
    Player player(textures["player.png"], CENTER, {30, 45});
    player.scale(4.4f, 4.4f);

    // Test walls
    Wall test({100, 300}, {600, 400});
    Wall test2({300, 100}, {1200, 700});
    Wall test3({100, 100}, {800, 900});
    Wall test4({100, 100}, {1650, 400});

    // Frame walls
    float bound_width = 300;
    Wall left_bound({bound_width, win_size.y}, {-bound_width / 2, win_size.y / 2});
    Wall right_bound({bound_width, win_size.y}, {win_size.x + bound_width / 2, win_size.y / 2});
    Wall low_bound({win_size.x, bound_width}, {win_size.x / 2, -bound_width / 2});
    Wall up_bound({win_size.x, bound_width}, {win_size.x / 2, win_size.y + bound_width / 2});

    // Test circle objs
    sf::RenderTexture transp;
    transp.create(30, 30);
    sf::CircleShape circle(transp.getSize().x);
    circle.setFillColor(sf::Color::Transparent);
    transp.draw(circle);

    std::array<DynamicObject, 50> circles;
    srand(time(nullptr));
    float start_pos = 40;
    float delta = window.getView().getSize().x / (circles.size() + 1);
    float offset = 0;

    for (auto &it : circles) {
        it = DynamicObject(transp.getTexture(), {start_pos + offset, 150});
        it.setMass(1);
        // sf::Vector2f v = {player.getPosition() - it.getPosition()};
        // it.setVelocity(v / 2.f);
        it.setVelocity({rand() % 200 - 100.f, rand() % 250 + 225.f});
        it.setHitbox(transp.getSize().x);
        offset += delta;
    }

    // std::array<DynamicObject, 25> rects;
    // offset = 0;
    // for (auto &it : rects) {
    //     it = DynamicObject(transp.getTexture(), {start_pos + offset, 230});
    //     it.setMass(1);
    //     it.setVelocity({rand() % 200 - 100.f, rand() % 250 + 150.f});
    //     it.setHitbox(static_cast<sf::Vector2f>(transp.getSize()));
    //     offset += delta;
    // }

    // std::array<DynamicObject, 25> circles2;
    // delta = window.getView().getSize().x / (circles2.size() + 1);

    // transp.create(50, 50);
    // sf::CircleShape circle2(transp.getSize().x);
    // circle2.setFillColor(sf::Color::Transparent);
    // transp.draw(circle2);
    // offset = 0;
    // start_pos += 15;

    // for (auto &it : circles2) {
    //     it = DynamicObject(transp.getTexture(), {start_pos + offset, 150});
    //     it.setMass(1);
    //     // sf::Vector2f v = {player.getPosition() - it.getPosition()};
    //     // it.setVelocity(v / 2.f);
    //     it.setVelocity({rand() % 200 - 100.f, rand() % 250 + 225.f});
    //     it.setHitbox(transp.getSize().x);
    //     offset += delta;
    // }

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

        // Collision checks
        DynamicObject::move_all(deltaTime);
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
