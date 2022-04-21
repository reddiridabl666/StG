#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "LoadTextures.hpp"
#include "Window.h"

#include <vector>

#define CENTER window.getSize().x / 2.f, window.getSize().y / 2.f
static constexpr sf::Uint8 player_layer = 1;
static constexpr sf::Uint8 layer_num = 3;

static const sf::Vector2f left(-1.0, 0.0);
static const sf::Vector2f right(1.0, 0.0);
static const sf::Vector2f up(0.0, -1.0);
static const sf::Vector2f down(0.0, 1.0);

bool is_fullscreen = true;
GameObject::objects GameObject::all_objects;
std::vector<GameObject::objects> GameObject::objects_by_layer(layer_num);

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

    // Background initialization
    GameObject bg(&textures["images/bg.jpg"]);
    bg.setPosition(CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor);

    // Player initialization
    GameObject player(&textures["images/player.png"], player_layer);
    player.setPosition(CENTER);
    player.scale(0.4f);

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                player.move(left * speed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                player.move(right * speed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                player.move(up * speed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                player.move(down * speed * deltaTime);
            }
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
