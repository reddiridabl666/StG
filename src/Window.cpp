#include "GameObject.h"
#include "Game.hpp"
#include "Window.h"

Window::Window(Game& game) : 
    sf::RenderWindow(),
    game(game) {
    show();
}

Window::Window(Game& game, sf::VideoMode mode, const sf::String &title,
               sf::Uint32 style, const sf::ContextSettings &settings) : 
    sf::RenderWindow(mode, title, style, settings),
    game(game) {
    show();
}

sf::Vector2f Window::getCenter() const {
    return static_cast<sf::Vector2f>(getSize()) / 2.f;
}

bool Window::is_fullscreen() {
    return is_fullscreen_;
}

void Window::pause() {
    if (game.paused) {
        game.in_loop = false;
        return;
    }
    if (game.in_game) {
        game.pause_menu();
    }
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
    sf::Event event;
    while (pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                close();
                break;
            case sf::Event::JoystickButtonReleased:
                if (event.joystickButton.button == JOY_ST) {
                    pause();
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Enter && event.key.alt) {
                        switch_view_mode();
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    pause();
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

// void Window::handle_event(sf::Event::EventType type, const std::function<void()>& handler) {
//     while (pollEvent(event)) {
//         if (event.type == type) {
//             handler();
//         }
//     }
// }

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
