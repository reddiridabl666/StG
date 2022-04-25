#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "HitboxObject.hpp"
#include "LoadTextures.hpp"
#include "Player.h"
#include "Window.h"

#include <vector>
#include <cassert>

#define CENTER static_cast<sf::Vector2f>(window.getSize()) / 2.f

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");
    
    // Init window
    Window window;
    // window.open_windowed();

    RectHitbox hitbox({50.f, 50.f}, {750.f, 500.f});
    CircleHitbox c_hitbox(100, {1200.f, 700.f});
    
    // Background initialization
    GameObject bg(textures["bg.jpg"]);
    bg.setPosition(CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor, factor);

    // Player initialization
    Player player(textures["player.png"], {30, 45});
    player.setPosition(CENTER);
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
            player.control(deltaTime);
        }

        // Collision checks
        Hitbox::check_collisions();

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here
        GameObject::draw_all(window);

        // end the current frame
        window.display();
    }
    return 0;
}
