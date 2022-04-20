#include <SFML/Graphics.hpp>
#include <GameObject.h>

#include <vector>

#define CENTER window.getSize().x / 2.f, window.getSize().y / 2.f
static constexpr uint32_t player_layer = 1;


const sf::Vector2f left(-1.0, 0.0);
const sf::Vector2f right(1.0, 0.0);
const sf::Vector2f up(0.0, -1.0);
const sf::Vector2f down(0.0, 1.0);

bool is_fullscreen = true;
GameObject::objects GameObject::all_objects;
std::vector<GameObject::objects> GameObject::objects_by_layer;

void open_fullscreen(sf::Window& window) {
    window.create(sf::VideoMode::getFullscreenModes()[0],
                  "My window",
                  sf::Style::Fullscreen);
    is_fullscreen = true;
}

void open_windowed(sf::Window& window) {
    // Потом надо научиться определять в зависимости от монитора
    window.create(sf::VideoMode(1280, 800),
                  "My window",
                  sf::Style::Titlebar | sf::Style::Close);
    is_fullscreen = false;
}

bool pressed_any_of(sf::Keyboard::Key A, sf::Keyboard::Key B) {
    return sf::Keyboard::isKeyPressed(A) ||
           sf::Keyboard::isKeyPressed(B);
}

float speed = 750.0;

int main()
{
    GameObject::objects_by_layer.resize(3);

    // Window initialization
    sf::RenderWindow window;
    open_fullscreen(window);
    window.setVerticalSyncEnabled(true);

    // Background initialization
    GameObject bg("images/bg.jpg");
    bg.setPosition(CENTER);
    auto factor = static_cast<float>(window.getSize().x) / bg.getSize().x;
    bg.scale(factor);

    // Player initialization
    GameObject player("images/arapuzz.png", player_layer);
    player.setPosition(CENTER);
    player.scale(0.4f);

    // Time
    sf::Clock clock;
    float deltaTime;

    // Event loop
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        // Windows events
        sf::Event event = {};
        while (window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                /*case sf::Event::Resized: {
                    auto w = static_cast<float>(event.size.width);
                    auto h = static_cast<float>(event.size.height);
                    window.setView(sf::View(sf::FloatRect(0, 0, w, h)));
                    break;
                }*/
                case sf::Event::KeyReleased:
                    if ((event.key.code == sf::Keyboard::Enter && event.key.alt) ||
                        event.key.code == sf::Keyboard::Escape) {

                        float width = static_cast<float>(window.getSize().x);
                        auto center = window.getView().getCenter();

                        if (is_fullscreen) {
                            open_windowed(window);
                        } else {
                            if (event.key.code != sf::Keyboard::Escape) {
                                open_fullscreen(window);
                            }
                        }

                        float new_width = static_cast<float>(window.getSize().x);
                        GameObject::scale_all(new_width / width);

                        sf::View new_view(center, window.getView().getSize());
                        window.setView(new_view);
                    }
                    break;
                default:
                    break;
            }
        }

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
