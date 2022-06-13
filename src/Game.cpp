#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"

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
    boss.lock()->hide();
    player.lock()->hide();
    player.lock()->getHealthBar().hide();

    main_menu();

    clock.restart();
    game_loop();
}

void Game::game_loop() {
#ifdef DEBUG
    Text timer(std::to_string(player->get_invinc_time()),
                Resources::fonts["ARIAL"], 48, {1650, 450});
#endif

    boss.lock()->show();
    player.lock()->show();
    player.lock()->getHealthBar().show();

    event_loop([this] {
        deltaTime = clock.restart().asSeconds();

        if (window.hasFocus() && !player.expired()) {
            player.lock()->update();
        }

        GameObject::update_all(deltaTime);

        check_collisions();
        
#ifdef DEBUG
        timer.setString(!player.expired() ? player->get_invinc_time() : 0);
#endif
    });
}

void Game::main_menu() {
    Button start("Start", 64, window, window.getCenter() - sf::Vector2f(0, 70), [this] {
        in_loop = false;
    });

    Button exit("Exit", 64, window, window.getCenter() + sf::Vector2f(0, 70), [this] {window.close();});

    event_loop();
}

void Game::pause_menu() {
    Button start("Back to game", 64, window, window.getCenter() - sf::Vector2f(0, 70), [this] {
        in_loop = false;
    });

    Button exit("Exit", 64, window, window.getCenter() + sf::Vector2f(0, 70), [this] {window.close();});

    event_loop();
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
