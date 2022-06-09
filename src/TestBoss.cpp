#include "TestBoss.hpp"
#include "Math.hpp"
#include "Random.hpp"

void TestBoss::BallBounce::shoot() {
    sf::Vector2f bullet_size = {150, 150};

    const size_t size = rand_gen(3, 5);
    const static sf::Vector2f start_pos = parent->getPosition() - (size / 2.f) * sf::Vector2f{bullet_size.x, 0};
    const static sf::Vector2f delta = {bullet_size.x, 0};
    sf::Vector2f offset = {};

    for (size_t i = 0; i < size; ++i) {
        gen().shoot(Bullet::Types[BulletType::BigCircle_Red], 
                        start_pos + offset, 
                        {rand_gen(-300.f, 300.f), rand_gen(25.f, 125.f)},
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
    const static auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    const static float delta = 140;
    const static int num = GameState::window()->getSize().x / delta + 1;
    // std::cout << GameState::window()->getSize().x  << " " << num << std::endl;
    const static auto start_pos = sf::Vector2f{50, parent->getPosition().y + 15};
    float offset = 0;
    
    shot_num()++;

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
    const static float radius = 200;
    const static auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    const static sf::Vector2f start_pos = parent->getPosition();
    const static size_t num = 46;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        float speed = 450;
        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * -speed;

        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
        bullet->setRotation(-to_degrees(arctan(velocity)));
    }
}

void TestBoss::Circular_1::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::Circular_2::shoot() {
    const static int radius = 150;
    const static auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    const static sf::Vector2f start_pos = parent->getPosition();
    const static size_t num = 30;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        float speed = 100;
        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;
        // velocity += sf::Vector2f(200, 0);

        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
        float rotate_speed = shot_num() / 8 % 2 ? 150 : -150;
        bullet->setRotation(-to_degrees(arctan(velocity)));
        bullet->setUpdateFunc(delete_when_out_of_bounds + circular(parent->getPosition(), rotate_speed));
    }
}

void TestBoss::Circular_2::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::CircularGrouped::shoot() {
    const static float radius = 150;
    const static auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    const static sf::Vector2f start_pos = parent->getPosition();
    const static size_t group_num = 8;
    const static size_t num = 6;

    float start_angle = rand_gen(0.f, 2 * constants::pi);
    static const float speed = 350;
    const static float delta = constants::pi / 14;
    
    shot_num()++;

    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        auto pos = start_pos + radius * sf::Vector2f{static_cast<float>(cos(start_angle)), 
                                                     static_cast<float>(sin(start_angle))};
        sf::Vector2f velocity = unit_vector(pos, parent->getPosition()) * speed;
        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], pos, velocity, bullet_size);
        bullet->setRotation(-to_degrees(arctan(velocity)));

        for (size_t j = 0; j < num / 2; ++j) {
            float x = start_pos.x + radius * cos(start_angle + delta * (j + 1));
            float y = start_pos.y + radius * sin(start_angle + delta * (j + 1));

            
            // sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
            bullet->setRotation(-to_degrees(arctan(velocity)));

            x = start_pos.x + radius * cos(start_angle - delta * (j + 1));
            y = start_pos.y + radius * sin(start_angle - delta * (j + 1));

            // velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
            bullet->setRotation(-to_degrees(arctan(velocity)));
        }
    }
}

void TestBoss::CircularGrouped::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

void TestBoss::Chaos::shoot() {
    const static float radius = 150;
    const static auto bullet_size = sf::Vector2f{12, 14} * 3.f;
    const static sf::Vector2f start_pos = parent->getPosition();
    const static size_t group_num = 8;
    const static size_t num = 6;

    float start_angle = rand_gen(0.f, 2 * constants::pi);
    static const float speed = 150;
    const static float delta = constants::pi / 14;
    
    shot_num()++;

    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        for (size_t j = 0; j < num / 2; ++j) {
            float x = start_pos.x + radius * cos(start_angle + delta * (j + 1));
            float y = start_pos.y + radius * sin(start_angle + delta * (j + 1));

            
            sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
            bullet->setRotation(-to_degrees(arctan(velocity)));

            x = start_pos.x + radius * cos(start_angle - delta * (j + 1));
            y = start_pos.y + radius * sin(start_angle - delta * (j + 1));

            velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity, bullet_size);
            bullet->setRotation(-to_degrees(arctan(velocity)));
        }
    }
}

void TestBoss::Chaos::update(float time) {
    if (time >= 0.7) {
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
            changePhase(new Chaos(this, 6000));
            break;
        case 3:
            changePhase(new ChessHoming(this, 5000));
            break;
        case 4:
            changePhase(new Circular_2(this, 5000));
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
