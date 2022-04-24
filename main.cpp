#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "HitboxObject.h"
#include "LoadTextures.hpp"
#include "Player.h"
#include "Window.h"

#include <vector>

#define CENTER window.getSize().x / 2.f, window.getSize().y / 2.f

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");
    
    // Init window
    Window window;

    RectHitbox hitbox({50.f, 50.f}, {750.f, 500.f});
    
    // Background initialization
    GameObject bg(textures["bg.jpg"]);
    bg.setPosition(CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor);

    // Player initialization
    Player player(textures["player.png"], {30, 45});
    player.setPosition(CENTER);
    player.scale(4.4f);

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
        if (player.collides_with(hitbox)) {
            player.hitbox_.on_collide();
            hitbox.on_collide();
        } else {
            player.hitbox_.on_collide_stop();
            hitbox.on_collide_stop();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here
        GameObject::draw_all(window);

        // end the current frame
        window.display();
    }
    return 0;
}
