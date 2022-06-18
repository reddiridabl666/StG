#include "GameObject.h"
#include "Controls.hpp"
#include "Game.hpp"
#include "Window.h"
#include "Menu.hpp"

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
    return getView().getCenter();
}

bool Window::is_fullscreen() {
    return is_fullscreen_;
}

void Window::reset_menu(Menu* menu) {
    menu_ = menu;
    pointer_ = -1;
}

void Window::back() {
    if (!game.in_main_menu && !((game.in_game + game.paused) % 2)) {
        game.in_loop = false;
        game.back_pressed = true;
    }
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

inline void Window::push_button() {
    if (pointer_ != -1) {
        menu_->buttons_[pointer_].activate();
    }
}

inline void Window::next_button() {
    menu_timer_.restart();
    hover_override = true;
    pointer_ = pointer_ < menu_->buttonNum() - 1 ? pointer_ + 1 : 0;
}

inline void Window::prev_button() {
    menu_timer_.restart();
    hover_override = true;
    pointer_ = pointer_ > 0 ? pointer_ - 1 : menu_->buttonNum() - 1;
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
    static float threshold = 15;

    if (!hover_override && menu_ && menu_->hover_ptr != -1) {
        pointer_ = menu_->hover_ptr;
    }

    sf::Event event;
    while (pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                close();
                break;

            case sf::Event::JoystickButtonPressed:
                if (event.joystickButton.button == Gamepad::START) {
                    pause();
                }
                if (event.joystickButton.button == Gamepad::B) {
                    back();
                }
                break;
            case sf::Event::MouseMoved:
                hover_override = false;
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    push_button();
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == Key::Enter &&
                    event.key.alt) {
                        switch_view_mode();
                }
                if (event.key.code == Key::Escape) {
                    pause();
                }
#ifdef DEBUG
                if (event.key.code == Key::Equal ||
                    event.key.code == Key::Add) {
                    auto new_view = getView();
                    new_view.zoom(0.8);
                    setView(new_view);
                }
                if (event.key.code == Key::Subtract ||
                    event.key.code == Key::Hyphen) {
                    auto new_view = getView();
                    new_view.zoom(1.25);
                    setView(new_view);
                }
#endif          
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == Key::Enter &&
                   !event.key.alt && menu_timer_.getElapsedTime().asSeconds() >= 0.05) {
                    menu_timer_.restart();
                    push_button();
                }
                if (!menu_ || menu_timer_.getElapsedTime().asSeconds() <= 0.1) {
                    break;
                }
                if (event.key.code == Key::W || event.key.code == Key::Up) {
                    prev_button();
                }
                if (event.key.code == Key::S || event.key.code == Key::Down) {
                    next_button();
                }
                break;
            default:
                break;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
       push_button();
    }

    if (Joy::isButtonPressed(0, Gamepad::A) && menu_timer_.getElapsedTime().asSeconds() > 0.05) {
        menu_timer_.restart();
        push_button();
    }
    if (menu_ && menu_timer_.getElapsedTime().asSeconds() > 0.12) {
        auto mv = gamepad_movement(Axis::Y, Axis::PovY, threshold);
        if (mv < 0) {
            prev_button();
        }
        if (mv > 0) {
            next_button();
        }
    }

    if (pointer_ != -1) {
        menu_->buttons_[pointer_].hover();
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
    create(sf::VideoMode(sf::VideoMode::getFullscreenModes()[0].width / 1.5,
           sf::VideoMode::getFullscreenModes()[0].height / 1.5),
           "STG",
           sf::Style::Default);
    is_fullscreen_ = false;
}
