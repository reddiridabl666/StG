#include "Game.hpp"

GameState GameState::state;

void Game::start() {
    // player->setHP(1);

    // Wall test({100, 300}, {600, 400});
    // Wall test2({300, 100}, {1200, 700});
    // Wall test3({100, 100}, {400, 900});
    // Wall test4({100, 100}, {1650, 300});

    // srand(time(nullptr));
    // size_t size = 15;
    // float start_pos = 40;
    // float delta = window.getView().getSize().x / (size + 1);
    // float offset = 0;

    // EnemyBulletGen test_gen;
    // for (size_t i = 0; i < size; ++i) {
    //     test_gen.shoot(Bullet::BulletTypes["test_circle"], 
    //                    {start_pos + offset, 150}, 
    //                    {rand() % 600 - 300.f, rand() % 100 + 25.f},
    //                    {150, 150});
    //     offset += delta;
    // }

    boss = new TestBoss(Resources::textures["boss"], {window.getCenter().x, 200}, sf::Vector2f{300.f, 300.f}, 1000);
    boss->scale(4.3, 4.3);
    
    // test_gen.for_each([] (Bullet* it) {it->scale(2, 2);});
    // test_gen.for_each([] (Bullet* it) {std::cout << "Size: " << it->getFrame()->getSize() << std::endl <<
    //                                              "Half Size: " << it->getFrame()->getHalfSize();});
    // test_gen.for_each([] (Bullet* it) {it->setVelocity(0, 0);});
    // test_gen.for_each([] (Bullet* it) {it->setUpdateFunc(delete_when_out_of_bounds);});

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

    boss->player_collision(player);
    BulletGenerator::check_collisions_with_bullets(player);
    
    for (auto it : frame.iter()) {
        FramedObject::check_phys_collisions_with(*it);
    }
}
