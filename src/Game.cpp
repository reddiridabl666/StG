#include "Game.hpp"
#include "TestBoss.hpp"
#include "Button.hpp"

GameState GameState::state;

void Game::start() {
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
        manager.update();
    
        window.sys_event_loop();

        if (window.hasFocus() && !player.expired()) {
            player.lock()->update();
        }

        GameObject::update_all(deltaTime);
        Ui::update_all();
        
        check_collisions();
        
#ifdef DEBUG
        timer.setString(!player.expired() ? player->get_invinc_time() : 0);
#endif

        window.clear(sf::Color::Black);
        DrawableObject::draw_all(window);

        window.display();
    }
}

void Game::main_menu() {

}

void Game::pause_menu() {
    
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
