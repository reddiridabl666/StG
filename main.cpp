#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "DynamicObject.hpp"
#include "Wall.hpp"
#include "LoadTextures.hpp"
#include "Player.h"
#include "Window.h"

#include <vector>
#include <cassert>

#define CENTER static_cast<sf::Vector2f>(window.getSize()) / 2.f

// Init window
Window window;
auto win_size = window.getView().getSize();

std::array<Wall, 4> Wall::Bounds = {
    Wall({1, win_size.y}, {0, win_size.y / 2}),  // left
    Wall({1, win_size.y}, {win_size.x, win_size.y / 2}),  // right
    Wall({win_size.x, 1}, {win_size.x / 2, 0}),  //upper
    Wall({win_size.x, 1}, {win_size.x / 2, win_size.y})  // down
};
int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");
    
    // Test rect hitbox_obj
    // sf::RenderTexture transp;
    // Wall wall({50, 50}, {750, 500});

    // Init screen borders
    

    // Wall left_wall({1, win_size.y}, {0, win_size.y / 2});
    // Wall right_wall({1, win_size.y}, {win_size.x, win_size.y / 2});
    // Wall up_wall({win_size.x, 1}, {win_size.x / 2, 0});
    // Wall down_wall({win_size.x, 1}, {win_size.x / 2, win_size.y});

    Wall test({100, 300}, {600, 400});
    Wall te2st({50, 100}, {1000, 400});

    // std::array<DynamicObject, 5> test_objs;
    // for (auto it : )

    // transp.create(50, 50);
    // sf::RectangleShape rect({50.f, 50.f});
    // rect.setFillColor(sf::Color::Transparent);
    // transp.draw(rect);
    // DynamicObject rect_obj(transp.getTexture(), {750, 500}, static_cast<sf::Vector2f>(transp.getSize()));

    // Test circle hitbox_obj
    sf::RenderTexture transp;
    transp.create(75, 75);
    sf::CircleShape circle(75);
    circle.setFillColor(sf::Color::Transparent);
    transp.draw(circle);
    DynamicObject circle_obj(transp.getTexture(), {1200, 680}, static_cast<sf::Vector2f>(transp.getSize()).x);

    // auto copy = circle_obj;
    // copy.move(-50, -250);

    // Background initialization
    GameObject bg(textures["bg.jpg"], CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor, factor);

    // Player initialization
    Player player(textures["player.png"], CENTER, {200, 300});
    player.scale(4.4f, 4.4f);

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
        // Hitbox::check_collisions();
        DynamicObject::move_all(deltaTime);
        DynamicObject::check_collisions_with(player);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here
        GameObject::draw_all(window);

        // end the current frame
        window.display();
    }
    return 0;
}
