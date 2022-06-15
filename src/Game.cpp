#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"
#include "Menu.hpp"

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

        window.clear(sf::Color::Black);
        DrawableObject::draw_all(window);

        window.display();
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
    in_menu = true;
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
    in_menu = false;
    start();
}

void Game::main_menu() {
    in_menu = true;
    bool settings_pressed = false;

    {
        Menu menu(Resources::textures["menu"], window.getCenter(), window, {
            {"Start", [this] {in_loop = false;}},
            {"Settings", [&] {in_loop = false; settings_pressed = true;}},
            {"Exit", [this] {window.close();}}});
        
        event_loop();
    }

    if (settings_pressed) {
        settings();
    }
    in_menu = false;
}

void Game::pause_menu() {
    paused = true;
    in_menu = true;

    bool settings_pressed = false;

    {
        Background gray(Resources::textures["pause"], window, Layer::Menu);

        Menu menu(Resources::textures["menu"], window.getCenter(), window, {
            {"Back to game", [this] {in_loop = false;}},
            {"Settings", [&] {in_loop = false; settings_pressed = true;}},
            {"Exit", [this] {window.close();}}}); 

        event_loop();
    }

    if (settings_pressed) {
        settings();
    }

    in_loop = true;
    paused = false;
    in_menu = false;
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

    {
        Background gray = gray_if_paused();

        Menu menu(Resources::textures["menu"], window.getCenter(), window, {
            {"Volume", [&] {in_loop = false; action = 0;}},
            {"Controls", [&] {in_loop = false; action = 1;}},
            {"Back", [&] {in_loop = false; action = 2;}}}); 

        event_loop();
    }

    switch (action) {
        case 0:
            volume();
            break;
        case 1:
            controls();
            break;
        case 2:
            if (paused)
                pause_menu();
            else
                main_menu();
            break;
        default:
            window.close();
    }
}

void Game::volume() {
    {
        Background gray = gray_if_paused();

        Menu menu(Resources::textures["menu"], window.getCenter(), window, {});
        menu.addButton({"^", [] {GameState::Settings().volumeUp();}}, 
                        menu.getPosition() + sf::Vector2f(215, -55), 108);
        auto& btn = menu.addButton({"^", [] {GameState::Settings().volumeDown();}}, 
                                    menu.getPosition() + sf::Vector2f(221, 45), 108);
        btn.setRotation(180);
        menu.addButton({"Back", [this] {in_loop = false;}});

        Log<sf::Uint16> volume("Volume: ", GameState::Settings().volume, menu.getPosition() + sf::Vector2f(-240, -50), 64);

        event_loop();
    }

    settings();
}

void Game::controls() {

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
