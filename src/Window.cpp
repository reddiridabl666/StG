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

sf::Vector2f Window::getCenter() const {
    return static_cast<sf::Vector2f>(getSize()) / 2.f;
}

bool Window::is_fullscreen() {
    return is_fullscreen_;
}

void Window::switch_view_mode() {
    float width = static_cast<float>(getSize().x);
    auto center = getView().getCenter();

    if (is_fullscreen()) {
        open_windowed();
    } else {
        open_fullscreen();
    }

    sf::View new_view(center, getView().getSize());
    if (!is_fullscreen_) new_view.zoom(width / getSize().x);
    setView(new_view);
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
                if (event.key.code == sf::Keyboard::Enter && event.key.alt) {
                        switch_view_mode();
                }
#ifdef DEBUG
                if (event.key.code == sf::Keyboard::Equal ||
                    event.key.code == sf::Keyboard::Add) {
                    auto new_view = getView();
                    new_view.zoom(0.8);
                    setView(new_view);
                }
                if (event.key.code == sf::Keyboard::Subtract ||
                    event.key.code == sf::Keyboard::Hyphen) {
                    auto new_view = getView();
                    new_view.zoom(1.25);
                    setView(new_view);
                }
#endif
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
    create(sf::VideoMode(sf::VideoMode::getFullscreenModes()[0].width / 1.5,
           sf::VideoMode::getFullscreenModes()[0].height / 1.5),
           "STG",
           sf::Style::Default);
            // sf::Style::Titlebar | sf::Style::Close);
    is_fullscreen_ = false;
}
