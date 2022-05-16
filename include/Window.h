#pragma once

#include "SFML/Graphics.hpp"

class Window : public sf::RenderWindow {
  public:
    bool is_fullscreen();

    Window();

    Window(sf::VideoMode mode,
            const sf::String &title,
            sf::Uint32 style = sf::Style::Default,
            const sf::ContextSettings &settings = sf::ContextSettings());

    sf::Vector2f getCenter() const;
    
    void sys_event_loop();
    void open_fullscreen(); 
    void open_windowed();

  private:
    void switch_view_mode();
    bool is_fullscreen_;
    void show();
};
