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

Frame Wall::Bounds = {
    Wall({1, win_size.y}, {0, win_size.y / 2}),  // left
    Wall({1, win_size.y}, {win_size.x, win_size.y / 2}),  // right
    Wall({win_size.x, 1}, {win_size.x / 2, 0}),  //up
    Wall({win_size.x, 1}, {win_size.x / 2, win_size.y})  // low
};

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");

    // Init screen borders

    // Wall test({100, 300}, {600, 400});
    // Wall te2st({50, 100}, {1000, 400});

    // std::array<DynamicObject, 5> test_objs;
    // for (auto it : )

    // transp.create(50, 50);
    // sf::RectangleShape rect({50.f, 50.f});
    // rect.setFillColor(sf::Color::Transparent);
    // transp.draw(rect);
    // DynamicObject rect_obj(transp.getTexture(), {750, 500}, static_cast<sf::Vector2f>(transp.getSize()));

    // Test circle hitbox_obj
    sf::RenderTexture transp;
    transp.create(30, 30);
    sf::CircleShape circle(transp.getSize().x);
    circle.setFillColor(sf::Color::Transparent);
    transp.draw(circle);

    // std::array<DynamicObject, 25> rects;
    // offset = 0;
    // for (auto &it : rects) {
    //     it = DynamicObject(transp.getTexture(), {start_pos + offset, 230});
    //     it.setMass(1);
    //     it.setVelocity({rand() % 200 - 100.f, rand() % 250 + 0.f});
    //     it.setHitbox(static_cast<sf::Vector2f>(transp.getSize()));
    //     offset += delta;
    // }

    // Background initialization
    GameObject bg(textures["bg.jpg"], CENTER);
    auto factor = std::max(static_cast<float>(window.getSize().x) / bg.getSize().x,
                           static_cast<float>(window.getSize().y) / bg.getSize().y);
    bg.scale(factor, factor);

    // Player initialization
    Player player(textures["player.png"], CENTER, {30, 45});
    player.scale(4.4f, 4.4f);

    // Test objs
    std::array<DynamicObject, 40> circles;
    srand(time(nullptr));
    float start_pos = 40;
    float delta = window.getView().getSize().x / (circles.size() + 1);
    float offset = 0;

    for (auto &it : circles) {
        it = DynamicObject(transp.getTexture(), {start_pos + offset, 150});
        it.setMass(1);
        sf::Vector2f v = {player.getPosition() - it.getPosition()};
        // it.setVelocity({rand() % 200 - 100.f, rand() % 350 + 0.f});
        it.setVelocity(v / 3.f);
        it.setHitbox(transp.getSize().x);
        offset += delta;
    }

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
