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

                    sf::View new_view(center, getView().getSize());
                    if (!is_fullscreen_) new_view.zoom(width / getSize().x);
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
            "STG",
            sf::Style::Fullscreen);
    is_fullscreen_ = true;
}

void Window::open_windowed() {
    // Потом надо научиться определять в зависимости от монитора
    create(sf::VideoMode(1280, 800),
           "STG",
           sf::Style::Default);
            // sf::Style::Titlebar | sf::Style::Close);
    is_fullscreen_ = false;
}
