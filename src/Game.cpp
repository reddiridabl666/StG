#include "Game.hpp"
#include "TestBoss.hpp"

GameState GameState::state;

void Game::start() {
    // player->setHP(1);
    
    boss = new TestBoss(Resources::textures["boss"], {window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f});
    boss->scale(4.3, 4.3);
    
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
    
        // OS events
        window.sys_event_loop();

        // Character control
        if (window.hasFocus() && player) {
            player->update();
        }

        // Movement and collisions
        // DynamicObject::move_all(deltaTime);
        GameObject::update_all(deltaTime);
        
        check_collisions();

        if (player && player->HP() <= 0) {
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

        // if (player == nullptr) {
        //     window.close();
        // }
    }
}

void Game::check_collisions() {
    DynamicObject::refresh_collision_num();
    if (boss) {
        boss->player_collision(player);
        boss->check_bullet_collisions(player);
    }
    // BulletGeneratorBase::check_collisions_with_bullets(player);
    
    for (auto it : frame.iter()) {
        // FramedObject::check_phys_collisions_with(*it);
        it->check_collisions_with(FramedObject::all);
        if (player) {
            it->player_collision(*player);
        }
    }
}
