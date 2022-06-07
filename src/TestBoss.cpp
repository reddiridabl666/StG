#include "TestBoss.hpp"

void TestBoss::BallBounce::shoot() {
    sf::Vector2f bullet_size = {150, 150};

    srand(time(nullptr));
    size_t size = rand() % 3 + 3;
    sf::Vector2f start_pos = parent->getPosition() - (size / 2.f) * sf::Vector2f{bullet_size.x, 0};
    sf::Vector2f delta = {bullet_size.x, 0}; /* GameState::window()->getView().getSize().x / (size + 1); */
    sf::Vector2f offset = {};

    for (size_t i = 0; i < size; ++i) {
        gen().shoot(Bullet::Types[BulletType::BigCircle_Red], 
                        start_pos + offset, 
                        {rand() % 600 - 300.f, rand() % 100 + 25.f},
                        bullet_size);
        offset += delta;
    }

    shot_num()++;
}

void TestBoss::BallBounce::update(float time) {
    if (shot_num() < 5 && time >= 1.2) {
        shoot();
        shoot_clock().restart();
    } else if (time >= 6) {
        shot_num() = 0;
        gen().for_each([](Bullet* it) {
            it->setMass(0);
        });
    }
}

void TestBoss::ChessHoming::shoot() {
    // Видимо, переменные закинуть в фазу
    auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    float delta = 140;
    int num = GameState::window()->getSize().x / delta + 1;
    // std::cout << GameState::window()->getSize().x  << " " << num << std::endl;
    auto start_pos = sf::Vector2f{50, parent->getPosition().y + 15};
    float offset = 0;
    
    shot_num()++;

    srand(time(nullptr));
    // auto modifier = rand() % 100 / 100.f;

    if (!(shot_num() % 2)) {
        sf::Vector2f velocity = unit_vector(GameState::getPlayerPos(), parent->getPosition()) * 1000.f;
        gen().shoot(Bullet::Types[BulletType::Circle_Red], parent->getPosition(), velocity, sf::Vector2f{28, 28} * 3.f);
    }

    for (int i = 0; i < num - shot_num() % 2; ++i) {
        gen().shoot(Bullet::Types[BulletType::Talisman_RB], 
                    start_pos + sf::Vector2f{offset + (delta / 2) * (shot_num() % 2), 25.f},
                    {0, 400}, bullet_size);
        offset += delta;
    }
}

void TestBoss::ChessHoming::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::Circular_1::shoot() {
    float radius = 200;
    auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    sf::Vector2f start_pos = parent->getPosition();
    size_t num = 30;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        float speed = 500;
        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;

        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
        bullet->rotate(angle / 2 * constants::pi * 180);
    }
}

void TestBoss::Circular_1::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::Circular_2::shoot() {
    float radius = 200;
    auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    sf::Vector2f start_pos = parent->getPosition();
    size_t num = 30;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        float speed = 500;
        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * -speed;

        gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
    }
}

void TestBoss::Circular_2::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::update(float deltaTime) {
    AnimatedBoss::update(deltaTime);
    
    if (hp_ <= 0) {
        switch (phase_num_) {
        case 1:
            changePhase(new BallBounce(this, 7000));
            break;
        case 2:
            changePhase(new Circular_2(this, 6000));
            break;
        case 3:
            changePhase(new ChessHoming(this, 5000));
            break;
        default:
            changePhase(nullptr);
            break;
        }
    }

    auto time = shoot_clock_.getElapsedTime().asSeconds();
#ifdef DEBUG
    time_ = time;
#endif
    if (phase_)
        phase_->update(time);
}
