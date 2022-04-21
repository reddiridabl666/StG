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
    
    void sys_event_loop();
    void open_fullscreen(); 
    void open_windowed();

  private:
    bool is_fullscreen_;
    void show();
};
