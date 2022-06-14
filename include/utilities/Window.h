#pragma once

#include "SFML/Graphics.hpp"
class Game;

class Window : public sf::RenderWindow {
public:
    bool is_fullscreen();

    Window(Game& game);

    Window(Game& game,
           sf::VideoMode mode,
           const sf::String &title,
           sf::Uint32 style = sf::Style::Default,
           const sf::ContextSettings &settings = sf::ContextSettings());

    sf::Vector2f getCenter() const;
    
    void sys_event_loop();
    // void handle_event(sf::Event::EventType type, const std::function<void()>& handler);
    void open_fullscreen(); 
    void open_windowed();

private:
    void pause();
    void switch_view_mode();
    Game& game;
    bool is_fullscreen_;
    void show();
};
