#include "Button.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "TestBoss.hpp"

void Game::Manager::update() {
    for (auto it = objs.begin(); it != objs.end();) {
        if (!(*it)->is_active()) {
            it = objs.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::Manager::refresh() {
    for (auto& it : objs) {
        if (it) {
            it->deactivate();
        }
    }
    update();
}

void Game::event_loop(const std::function<void()>& action) {
    in_loop = true;
    
    while (window.isOpen() && in_loop) {
        Ui::update_all();
        window.sys_event_loop();
        manager.update();

        action();

        DrawableObject::draw_all(window);
    }
}

void Game::start() {
    manager.refresh();

    main_menu();

    player = manager.add(std::make_shared<Player>(window.getCenter(), sf::Vector2f{25, 30}));
    boss = manager.add(std::make_shared<TestBoss>(sf::Vector2f{window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f}));

    GameState::setPlayerTracking(player);
    
    clock.restart();
    game_loop();
}

void Game::game_loop() {
    in_game = true;

#ifdef DEBUG
    Text timer(std::to_string(player.lock()->get_invinc_time()),
                Resources::fonts["ARIAL"], 48, {1650, 450});
#endif

    event_loop([&] {
        deltaTime = clock.restart().asSeconds();

        if (window.hasFocus() && !player.expired()) {
            player.lock()->update();
        }

        if (player.expired() || boss.expired()) {
            in_loop = false;
        }

        GameObject::update_all(deltaTime);

        check_collisions();
        
#ifdef DEBUG
        timer.setString(!player.expired() ? player.lock()->get_invinc_time() : 0);
#endif
    });

    if (window.isOpen())
        game_over();
}

void Game::game_over() {
    in_game = false;
    {
        Background gray(Resources::textures["pause"], window, Layer::Menu);
        
        const sf::String text = [this] {
            if (player.expired()) {
                return "Game Over";
            } else {
                return "You won!";
            }
        }();

        LabeledMenu end(text, Resources::textures["menu"],
                    window.getCenter(), window, 
                    {{"Back to menu", [this] {in_loop = false;}},
                     {"Exit", [this] {window.close();}}});
        event_loop();
    }
    start();
}

void Game::menu(sf::String text, const std::vector<Button::Info>& infos) {
    Background gray = gray_if_paused();

    LabeledMenu menu_(text, Resources::textures["menu_large"], window.getCenter(), window, infos);

    event_loop();
}

void Game::main_menu() {
    bool settings_pressed = false;

    menu("Main menu", {
         {"Start", [this] {in_loop = false;}},
         {"Settings", [&] {in_loop = false; settings_pressed = true;}},
         {"Exit", [this] {window.close();}}});

    if (settings_pressed) {
        settings();
    }
}

void Game::pause_menu() {
    paused = true;
    bool settings_pressed = false;

    menu("Pause", {
        {"Back to game", [this] {in_loop = false;}},
        {"Settings", [&] {in_loop = false; settings_pressed = true;}},
        {"Exit", [this] {window.close();}}});
   
    if (settings_pressed) {
        settings();
    }

    in_loop = true;
    paused = false;
    clock.restart().asSeconds();
}

inline Background Game::gray_if_paused() {
    if (paused)
        return Background(Resources::textures["pause"], window, Layer::Menu);
    else
        return Background();
}

void Game::settings() {
    int action = -1;

    menu("Settings", {
         {"Volume", [&] {in_loop = false; action = 0;}},
         {"Controls", [&] {in_loop = false; action = 1;}},
         {"Back", [&] {in_loop = false; action = 2;}}}); 

    switch (action) {
        case 0:
            volume();
            Settings::update();
            break;
        case 1:
            controls();
            Settings::update();
            break;
        case 2:
            if (paused)
                pause_menu();
            else
                main_menu();
            break;
        default:
            break;
    }
}

void Game::volume() {
    bool back_pressed = false;

    {
        Background gray = gray_if_paused();

        static std::function<std::string()> refreshed = [] {
            return "Volume: " + std::to_string(Settings::volume());
        };

        Text volume(refreshed(), window.getCenter() + sf::Vector2f(-255, -65), 72);

        Menu menu(Resources::textures["menu"], window.getCenter(), window, {});
        menu.addButton({"^", [&] {Settings::volumeUp();
                                 volume.setString(refreshed());}}, 
                        menu.getPosition() + sf::Vector2f(225, -70), 108);

        auto& btn = menu.addButton({"^", [&] {Settings::volumeDown();
                                             volume.setString(refreshed());}}, 
                                    menu.getPosition() + sf::Vector2f(231, 30), 108);
        btn.setRotation(180);

        menu.addButton({"Back", [&] {in_loop = false; back_pressed = true;}});

        event_loop();
    }
    if (back_pressed)
        settings();
}   

void Game::controls() {
    int action = -1;

    menu("Controls", {
         {"Keyboard", [&] {in_loop = false; action = 0;}},
         {"Gamepad", [&] {in_loop = false; action = 1;}},
         {"Back", [&] {in_loop = false; action = 2;}}}); 

    if (action == 0) {
        keyboard();
    }

    if (action == 1) {
        gamepad();
    }

    if (action == 2) {
        settings();
    }
}

// void Game::settings_menu(sf::String text, const std::vector<Button::Info>& infos) {
    // Background gray = gray_if_paused();

    // LabeledMenu menu_(text, Resources::textures["menu_large"], window.getCenter(), window, infos);
    // std::vector<Log<std::string>> logs;

    // for (auto it = menu_.getButtons().begin(); it != std::prev(menu_.getButtons().end()); ++it) {
    //     logs.emplace_back("", Settings::);
    // }

    // event_loop();
// }

void Game::keyboard() {
    bool back_pressed = false;
    {
        Background gray = gray_if_paused();

        // bool shoot_changed = false;
        // bool slow_changed = false;

        sf::Vector2f delta = {-100, 0};

        CenteredText shoot(key_to_str(Settings::getKey("shoot")), window.getCenter() + 
                               sf::Vector2f(130, -45));
        CenteredText slow(key_to_str(Settings::getKey("slow")), shoot.getPosition() + 
                              sf::Vector2f(0, 100));

        // TODO: Убрать копипасту
        LabeledMenu menu("Keyboard", Resources::textures["menu_large"], 
                        window.getCenter(), window, 
                        {{"Shoot:", [&] {shoot.setString("---");
                                         DrawableObject::draw_all(window);
                                         auto res = get_key(window);
                                         if (res == Key::Unknown)
                                            res = Settings::getKey("shoot");
                                         Settings::setKey("shoot", res);
                                         shoot.setString(key_to_str(res));
                                         }, delta},
                         {"Slow:", [&] {slow.setString("---");
                                        DrawableObject::draw_all(window);
                                        auto res = get_key(window);
                                        if (res == Key::Unknown)
                                            res = Settings::getKey("slow");
                                        Settings::setKey("slow", res);
                                        slow.setString(key_to_str(res));
                                        }, delta},
                         {"Back", [&] {in_loop = false; back_pressed = true;}}});

        event_loop();
    }

    if (back_pressed)
        controls();
}

void Game::gamepad() {
    bool back_pressed = false;
    {
        Background gray = gray_if_paused();
        sf::Clock timer;

        sf::Vector2f delta = {-100, 0};

        CenteredText shoot(std::to_string(Settings::getButton("shoot")), window.getCenter() + 
                               sf::Vector2f(130, -45));
        CenteredText slow(std::to_string(Settings::getButton("slow")), shoot.getPosition() + 
                              sf::Vector2f(0, 100));

        // TODO: Убрать копипасту
        LabeledMenu menu("Gamepad", Resources::textures["menu_large"], 
                        window.getCenter(), window, 
                        {{"Shoot:", [&] {if (timer.getElapsedTime().asSeconds() < 0.5)
                                            return;
                                         shoot.setString("---");
                                         DrawableObject::draw_all(window);
                                         auto res = get_button(window);
                                         if (res == Gamepad::ERR) {
                                            res = Settings::getButton("shoot");
                                         }
                                         Settings::setButton("shoot", res);
                                         shoot.setString(std::to_string(res));
                                         timer.restart();}, 
                                         delta},
                         {"Slow:", [&] {if (timer.getElapsedTime().asSeconds() < 0.7)
                                            return;
                                         slow.setString("---");
                                         DrawableObject::draw_all(window);
                                         auto res = get_button(window);
                                         if (res == Gamepad::ERR) {
                                            res = Settings::getButton("slow");
                                         }
                                         Settings::setButton("slow", res);
                                         slow.setString(std::to_string(res));
                                         timer.restart();}, 
                                         delta},
                         {"Back", [&] {in_loop = false; back_pressed = true;}}});

        event_loop();
    }

    if (back_pressed)
        controls();
}

void Game::check_collisions() {
    DynamicObject::refresh_collision_num();

    if (player.expired()) {
        return;
    }

    if (!boss.expired()) {
        boss.lock()->player_collision(player.lock());
        boss.lock()->check_bullet_collisions(player.lock().get());
    }
    
    for (auto it : frame.iter()) {
        it->check_collisions_with(FramedObject::all);
        if (!player.expired()) {
            it->player_collision(*player.lock());
        }
    }
}
