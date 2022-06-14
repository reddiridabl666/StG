#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"
#include "Menu.hpp"

GameState GameState::state;

using kb = sf::Keyboard;

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
        window.sys_event_loop();
        manager.update();

        Ui::update_all();

        action();

        window.clear(sf::Color::Black);
        DrawableObject::draw_all(window);

        window.display();
    }
}

void Game::start() {
    // std::cout << "I'm in start!  " << (window.isOpen() ? "true" : "false") << "\n";

    manager.refresh();

    main_menu();

    player = manager.add(std::make_shared<Player>(window.getCenter(), sf::Vector2f{25, 30}));
    boss = manager.add(std::make_shared<TestBoss>(sf::Vector2f{window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f}));
    GameState::init(player, &window, frame);

    // std::cout << "Going to the game loop!  " << (window.isOpen() ? "true" : "false") << "\n";
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

        // if (kb::isKeyPressed(kb::Key::Escape)) {
        //     pause_menu();
        // }
        
        // window.handle_event(sf::Event::EventType::KeyReleased, [this] {
        //     if (window.event.key.code == kb::Escape) {
        //         pause_menu();
        //     }
        // });

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

        EndScreen end(text, Resources::textures["menu"],
                    window.getCenter(), window, 
                    {{"Back to menu", [this] {in_loop = false;}}, 
                    {"Exit", [this] {window.close();}}});
        event_loop();
    }
    start();
}

void Game::main_menu() {
    // std::cout << "I'm in main menu!  " << (window.isOpen() ? "true" : "false") << "\n"; 
    Menu menu(Resources::textures["menu"], window.getCenter(), window, {
        {"Start", [this] {in_loop = false;}}, 
        {"Exit", [this] {window.close();}}}); 
    
    event_loop();
}

void Game::pause_menu() {
    paused = true;

    Background gray(Resources::textures["pause"], window, Layer::Menu);

    Menu menu(Resources::textures["menu"], window.getCenter(), window, {
        {"Back to game", [this] {in_loop = false;}}, 
        {"Exit", [this] {window.close();}}}); 

    event_loop(/* [this] {
        window.handle_event(sf::Event::EventType::KeyReleased, [this] {
            if (window.event.key.code == kb::Escape) {
                in_loop = false;
            }
    });} */);

    in_loop = true;
    paused = false;
    clock.restart().asSeconds();
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
