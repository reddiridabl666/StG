#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"
#include "Menu.hpp"

GameState GameState::state;

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

    if (!player.expired()) {
        player.lock()->deactivate();
    }
    if (!boss.expired()) {
        boss.lock()->deactivate();
    }
    manager.update();

    player = manager.add(std::make_shared<Player>(window.getCenter(), sf::Vector2f{25, 30}));
    boss = manager.add(std::make_shared<TestBoss>(sf::Vector2f{window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f}));
    GameState::init(player, &window, frame);

    boss.lock()->hide();
    player.lock()->hide();
    player.lock()->getHealthBar().hide();

    main_menu();

    // std::cout << "Going to the game loop!  " << (window.isOpen() ? "true" : "false") << "\n";
    clock.restart();
    game_loop();
}

void Game::game_loop() {
#ifdef DEBUG
    Text timer(std::to_string(player.lock()->get_invinc_time()),
                Resources::fonts["ARIAL"], 48, {1650, 450});
#endif

    boss.lock()->show();
    player.lock()->show();
    player.lock()->getHealthBar().show();

    event_loop([&] {
        deltaTime = clock.restart().asSeconds();

        if (window.hasFocus() && !player.expired()) {
            player.lock()->update();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            pause_menu();
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
                    {{"Back to menu", [this] {in_loop = false;}}, {"Exit", [this] {window.close();}}});
        event_loop();
    }
    start();
}

void Game::main_menu() {
    // std::cout << "I'm in main menu!  " << (window.isOpen() ? "true" : "false") << "\n"; 
    Menu menu(Resources::textures["menu"], window.getCenter(), window, {
        {"Start", [this] {in_loop = false;}}, {"Exit", [this] {window.close();}}}); 
    
    event_loop();
}

void Game::pause_menu() {
    Background gray(Resources::textures["pause"], window, Layer::Menu);

    Menu menu(Resources::textures["menu"], window.getCenter(), window, {
        {"Back to game", [this] {in_loop = false;}}, {"Exit", [this] {window.close();}}}); 

    event_loop();

    in_loop = true;
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
