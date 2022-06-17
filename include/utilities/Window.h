#pragma once

#include "SFML/Graphics.hpp"
class Game;
class Menu;

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
    void open_fullscreen(); 
    void open_windowed();

    void reset_menu(Menu*);

private:
    ssize_t pointer_ = -1;
    Menu* menu_ = nullptr;
    sf::Clock menu_timer_;

    void back();
    void pause();
    void switch_view_mode();

    void push_button();
    void next_button();
    void prev_button();

    Game& game;
    bool is_fullscreen_;
    bool hover_override = false;
    void show();

    friend class Menu;
};
