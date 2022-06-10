#include "TestBoss.hpp"
#include "Math.hpp"
#include "Random.hpp"

TestBoss::BallBounce::BallBounce(Boss* parent) : Phase(parent, 7000) {
    auto& info = Bullet::Types[BulletType::BigCircle_Red];
    num = rand_gen(3, 5);
    start_pos = parent->getPosition() - (num / 2.f) * sf::Vector2f{info.size.x, 0};
    delta = info.size.x;
}

void TestBoss::BallBounce::shoot() {
    for (size_t i = 0; i < num; ++i) {
        gen().shoot(Bullet::Types[BulletType::BigCircle_Red], 
                    start_pos + sf::Vector2f{delta * i, 0}, 
                    {rand_gen(-300.f, 300.f), rand_gen(25.f, 125.f)});
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

TestBoss::ChessHoming::ChessHoming(Boss* parent) : Phase(parent, 5000) {
    delta = 140;
    num = GameState::window()->getSize().x / delta + 1;
    start_pos = sf::Vector2f{50, parent->getPosition().y + 15};
}

void TestBoss::ChessHoming::shoot() {
    shot_num()++;

    if (!(shot_num() % 2)) {
        sf::Vector2f velocity = unit_vector(GameState::getPlayerPos(), parent->getPosition()) * 1000.f;
        gen().shoot(Bullet::Types[BulletType::Circle_Red], parent->getPosition(), velocity);
    }

    for (size_t i = 0; i < num - shot_num() % 2; ++i) {
        gen().shoot(Bullet::Types[BulletType::Talisman_RB], 
                    start_pos + sf::Vector2f{delta * i + (delta / 2) * (shot_num() % 2), 25.f},
                    {0, 400});
    }
}

void TestBoss::ChessHoming::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

TestBoss::Circular_1::Circular_1(Boss* parent) : Phase(parent, 6000) {
    num = 46;
}

void TestBoss::Circular_1::shoot() {
    static const float radius = 200;
    static const float speed = 400;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * -speed;

        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
        bullet->setRotation(-to_degrees(arctan(velocity)));
    }
}

void TestBoss::Circular_1::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

TestBoss::CircularRotating::CircularRotating(Boss* parent) : Phase(parent, 7000) {
    num = 30;
}

void TestBoss::CircularRotating::shoot() {
    static const int radius = 150;
    static const float speed = 100;

    shot_num()++;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        float x = start_pos.x + radius * cos(angle + constants::pi / num * (shot_num() % 2));
        float y = start_pos.y + radius * sin(angle + constants::pi / num * (shot_num() % 2));

        sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;

        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
        float rotate_speed = shot_num() / 8 % 2 ? 150 : -150;
        bullet->setRotation(-to_degrees(arctan(velocity)));
        bullet->setUpdateFunc(circular(parent->getPosition(), rotate_speed));
    }
}

void TestBoss::CircularRotating::update(float time) {
    if (time >= 0.5) {
        shoot();
        shoot_clock().restart();
    }
}

TestBoss::CircularGrouped::CircularGrouped(Boss* parent) : Phase(parent, 6000) {
    num = 6;
    delta = constants::pi / 14;
}

void TestBoss::CircularGrouped::shoot() {
    static const float radius = 150;
    static const size_t group_num = 8;

    float start_angle = rand_gen(0.f, 2 * constants::pi);
    static const float speed = 350;
    
    shot_num()++;

    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        auto pos = start_pos + radius * sf::Vector2f{static_cast<float>(cos(start_angle)), 
                                                     static_cast<float>(sin(start_angle))};
        sf::Vector2f velocity = unit_vector(pos, parent->getPosition()) * speed;
        auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], pos, velocity);
        bullet->setRotation(-to_degrees(arctan(velocity)));

        for (size_t j = 0; j < num / 2; ++j) {
            float x = start_pos.x + radius * cos(start_angle + delta * (j + 1));
            float y = start_pos.y + radius * sin(start_angle + delta * (j + 1));

            auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
            bullet->setRotation(-to_degrees(arctan(velocity)));

            x = start_pos.x + radius * cos(start_angle - delta * (j + 1));
            y = start_pos.y + radius * sin(start_angle - delta * (j + 1));

            bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
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

TestBoss::Chaos::Chaos(Boss* parent) : Phase(parent, 7000) {
    num = 6;
    delta = constants::pi / 14;
}


void TestBoss::Chaos::shoot() {
    static const float radius = 150;
    static const size_t group_num = 8;

    float start_angle = rand_gen(0.f, 2 * constants::pi);
    static const float speed = 150;

    shot_num()++;

    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        for (size_t j = 0; j < num / 2; ++j) {
            float x = start_pos.x + radius * cos(start_angle + delta * (j + 1));
            float y = start_pos.y + radius * sin(start_angle + delta * (j + 1));

            
            sf::Vector2f velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            auto bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
            bullet->setRotation(-to_degrees(arctan(velocity)));

            x = start_pos.x + radius * cos(start_angle - delta * (j + 1));
            y = start_pos.y + radius * sin(start_angle - delta * (j + 1));

            velocity = unit_vector({x, y}, parent->getPosition()) * speed;

            bullet = gen().shoot(Bullet::Types[BulletType::Talisman_RB], {x, y}, velocity);
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
            changePhase(new BallBounce(this));
            break;
        case 2:
            changePhase(new Chaos(this));
            break;
        case 3:
            changePhase(new ChessHoming(this));
            break;
        case 4:
            changePhase(new CircularGrouped(this));
            break;
        case 5:
            changePhase(new CircularRotating(this));
            break;
        default:
            setAnimation(sprites_["death"]);
            changePhase(nullptr);
            break;
        }
    }

    if (getTexture() == &sprites_["death"][7]) {
        deactivate();
    }
    
    auto time = shoot_clock_.getElapsedTime().asSeconds();
#ifdef DEBUG
    time_ = time;
#endif
    if (phase_)
        phase_->update(time);
}
