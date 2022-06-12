#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"

GameState GameState::state;

void Game::start() {
    boss = manager.add(new TestBoss({window.getCenter().x, 200}, sf::Vector2f{400.f, 200.f}));
    Button exit("Exit", window, {1700, 950}, [this] {window.close();});
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

        if (window.hasFocus() && !player.expired()) {
            player.lock()->update();
        }

        GameObject::update_all(deltaTime);
        
        check_collisions();

        // if (player && !player->is_active()) {
        //     delete player;
        //     player = nullptr;
        // }

        // if (boss && !boss->is_active()) {
        //     delete boss;
        //     boss = nullptr;
        // }

        GameState::update(player.lock().get());

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

    if (!boss.expired()) {
        boss.lock()->player_collision(player.lock().get());
        boss.lock()->check_bullet_collisions(player.lock().get());
    }
    
    for (auto it : frame.iter()) {
        it->check_collisions_with(FramedObject::all);
        if (!player.expired()) {
            it->player_collision(*player.lock().get());
        }
    }
}
