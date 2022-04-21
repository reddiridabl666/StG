#include "GameObject.h"
#include "Window.h"

Window::Window() : sf::RenderWindow() {
    show();
}

Window::Window(sf::VideoMode mode, const sf::String &title,
        sf::Uint32 style, const sf::ContextSettings &settings) 
        : sf::RenderWindow(mode, title, style, settings) {
    show();
}

bool Window::is_fullscreen() {
    return is_fullscreen_;
}
void Window::sys_event_loop() {
    // OS events
    sf::Event event = {};
    while (pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                close();
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

                    float width = static_cast<float>(getSize().x);
                    auto center = getView().getCenter();

                    if (is_fullscreen()) {
                        open_windowed();
                    } else {
                        if (event.key.code != sf::Keyboard::Escape) {
                            open_fullscreen();
                        }
                    }

                    float new_width = static_cast<float>(getSize().x);
                    GameObject::scale_all(new_width / width);

                    sf::View new_view(center, getView().getSize());
                    setView(new_view);
                }
                break;
            default:
                break;
        }
    }
}

void Window::show() {
    open_fullscreen();
    setVerticalSyncEnabled(true);
}

void Window::open_fullscreen() {
    create(sf::VideoMode::getFullscreenModes()[0],
            "My window",
            sf::Style::Fullscreen);
    is_fullscreen_ = true;
}

void Window::open_windowed() {
    // Потом надо научиться определять в зависимости от монитора
    create(sf::VideoMode(1280, 800),
            "My window",
            sf::Style::Titlebar | sf::Style::Close);
    is_fullscreen_ = false;
}
