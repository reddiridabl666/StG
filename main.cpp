#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "GameObject.h"
#include "HitboxObject.h"
#include "LoadTextures.hpp"
#include "Player.h"
#include "Window.h"

#include <vector>

#define CENTER window.getSize().x / 2.f, window.getSize().y / 2.f


GameObject::objects GameObjectBase::all_objects;
std::unordered_map<Layer, GameObject::objects> GameObjectBase::objects_by_layer(layer_num);

bool pressed_any_of(sf::Keyboard::Key A, sf::Keyboard::Key B) {
    return sf::Keyboard::isKeyPressed(A) ||
           sf::Keyboard::isKeyPressed(B);
}

float speed = 750.0;

int main()
{
    // Load all textures from a folder
    auto textures = load_textures("images");
    
    // Init window
    Window window;

    RectHitbox hitbox({50.f, 50.f}, {750.f, 500.f});

    // Background initialization
    GameObject bg(textures["images/bg.jpg"]);
    bg.setPosition(CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor);

    // Player initialization
    Player player(textures["images/player.png"], {25, 40}, Layer::character);
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

        // Keyboard events

        if (window.hasFocus()) {
            player.control(deltaTime);
            // if (pressed_any_of(Key::A, Key::Left)) {
            //     player.move(left * speed * deltaTime);
            // }
            // if (pressed_any_of(Key::D, Key::Right)) {
            //     player.move(right * speed * deltaTime);
            // }
            // if (pressed_any_of(Key::W, Key::Up)) {
            //     player.move(up * speed * deltaTime);
            // }
            // if (pressed_any_of(Key::S, Key::Down)) {
            //     player.move(down * speed * deltaTime);
            // }
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
