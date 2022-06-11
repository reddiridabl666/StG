#include "Game.hpp"
#include "TestBoss.hpp"

GameState GameState::state;

void Game::start() {
    boss = new TestBoss({window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f});
    clock.restart();
    event_loop();
}

void Game::event_loop() {
#ifdef DEBUG
    Text timer(std::to_string(player->get_invinc_time()),
                Resources::fonts["ARIAL"], 48, {1650, 450});
#endif
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
    
        window.sys_event_loop();

        if (window.hasFocus() && player) {
            player->update();
        }

        GameObject::update_all(deltaTime);
        
        check_collisions();

        if (player && !player->is_active()) {
            delete player;
            player = nullptr;
        }

        if (boss && !boss->is_active()) {
            delete boss;
            boss = nullptr;
        }

        GameState::update(player);

#ifdef DEBUG
        timer.setString(player ? player->get_invinc_time() : 0);
#endif

        window.clear(sf::Color::Black);
        GameObject::draw_all(window);

        window.display();
    }
}

void Game::check_collisions() {
    DynamicObject::refresh_collision_num();

    if (boss) {
        boss->player_collision(player);
        boss->check_bullet_collisions(player);
    }
    
    for (auto it : frame.iter()) {
        it->check_collisions_with(FramedObject::all);
        if (player) {
            it->player_collision(*player);
        }
    }
}
