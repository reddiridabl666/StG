#include "TestBoss.hpp"
#include "Math.hpp"
#include "Random.hpp"

Animated::Sprites TestBoss::init_sprites(const sf::Image& sprite_sheet)  {
    Animated::Sprites sprites_;
    sprites_["idle"] = load_row(sprite_sheet, 4, {0, 11}, {64, 40});
    sprites_["death"] = load_row(Resources::sprite_sheets["explosion"], 8, {0, 12}, {64, 40});
    return sprites_;
}

TestBoss::BallBounce::BallBounce(Boss* parent) : Phase(parent, 7000, 1.2) {
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
}

void TestBoss::BallBounce::update(float time) {
    if (shot_num() < 5 && time >= shot_interval) {
        parent->shoot();
        shoot_clock().restart();
    } else if (time >= 6) {
        shot_num() = 0;
        gen().for_each([](Bullet* it) {
            it->setMass(0);
        });
    }
}

TestBoss::BallBounce::~BallBounce() {
    gen().for_each([](Bullet* it) {
        it->setMass(0);
    });
}

TestBoss::ChessHoming::ChessHoming(Boss* parent) : Phase(parent, 5000) {
    delta = 140;
    num = GameState::window()->getSize().x / delta + 1;
    start_pos = sf::Vector2f{50, parent->getPosition().y + 15};
}

void TestBoss::ChessHoming::shoot() {
    speed = 400;

    if (!(shot_num() % 2)) {
        sf::Vector2f velocity = unit_vector(GameState::getPlayerPos(), parent->getPosition()) * 1000.f;
        gen().shoot(Bullet::Types[BulletType::Circle_Red], parent->getPosition(), velocity);
    }

    for (size_t i = 0; i < num - shot_num() % 2; ++i) {
        gen().shoot(Bullet::Types[BulletType::Talisman_RB], 
                    start_pos + sf::Vector2f{delta * i + (delta / 2) * (shot_num() % 2), 25.f},
                    down * speed);
    }
}

TestBoss::Circular_1::Circular_1(Boss* parent) : Phase(parent, 5500) {
    num = 46;
    speed = -400;
}

void TestBoss::Circular_1::shoot() {
    static const float radius = 200;
    
    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        shoot_circular(BulletType::Talisman_RB, radius, angle + constants::pi / num * (shot_num() % 2));
    }
}

TestBoss::CircularRotating::CircularRotating(Boss* parent) : Phase(parent, 6000) {
    num = 30;
    speed = 100;
}

void TestBoss::CircularRotating::shoot() {
    static const int radius = 150;

    for (size_t i = 0; i < num; ++i) {
        float angle = (2 * constants::pi) / num * i;
        auto bullet = shoot_circular(BulletType::Talisman_RB, radius, angle + constants::pi / num * (shot_num() % 2));

        float rotate_speed = shot_num() / 8 % 2 ? 150 : -150;
        bullet->setUpdateFunc(circular(parent->getPosition(), rotate_speed));
    }
}

TestBoss::CircularGrouped::CircularGrouped(Boss* parent) : Phase(parent, 6000) {
    num = 6;
    delta = constants::pi / 14;
    speed = 350;
}

void TestBoss::CircularGrouped::shoot() {
    static const float radius = 150;
    static const size_t group_num = 8;

    float start_angle = rand_gen(0.f, 2 * constants::pi);
    
    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        auto velocity = shoot_circular(BulletType::Talisman_RB, radius, start_angle)->getVelocity();

        for (size_t j = 0; j < num / 2; ++j) {
            shoot_circular(BulletType::Talisman_RB, radius, start_angle + delta * (j + 1), velocity);
            shoot_circular(BulletType::Talisman_RB, radius, start_angle - delta * (j + 1), velocity);
        }
    }
}

TestBoss::Chaos::Chaos(Boss* parent) : Phase(parent, 7000, 0.7) {
    num = 6;
    delta = constants::pi / 14;
    speed = 150;
}

void TestBoss::Chaos::shoot() {
    static const float radius = 150;
    static const size_t group_num = 8;

    float start_angle = rand_gen(0.f, 2 * constants::pi);

    for (size_t i = 0; i < group_num; ++i) {
        start_angle += 2 * constants::pi / group_num;

        for (size_t j = 0; j < num / 2; ++j) {
            shoot_circular(BulletType::Talisman_RB, radius, start_angle + delta * (j + 1));
            shoot_circular(BulletType::Talisman_RB, radius, start_angle - delta * (j + 1));
        }
    }
}

TestBoss::StreamsRandom::StreamsRandom(Boss* parent) : Phase(parent, 7000, 0.1) {
    num = 6;
    delta = 2 * constants::pi / num;
    speed = 300;
}

void TestBoss::StreamsRandom::shoot() {
    static const float radius = 150;
    const float start_angle = rand_gen(0.f, 2 * constants::pi);

    for (size_t i = 0; i < num; ++i) {
        shoot_circular(BulletType::Talisman_RB, radius, start_angle + delta * i);
    }
}

TestBoss::StreamsCircular::StreamsCircular(Boss* parent) : Phase(parent, 7000, 0.07) {
    num = 8;
    delta = 2 * constants::pi / num;
    speed = 500;
}

void TestBoss::StreamsCircular::shoot() {
    static const float radius = 150;
    static const float start_angle = rand_gen(0.f, 2 * constants::pi);

    static float offset = 0;
    size_t n = 60;
    offset += [n, modifier = shot_num() % n] {
        if (modifier < n / 6) {
            return 0.04;
        }
        if (modifier < n / 3) {
            return -0.02;
        }
        if (modifier < n / 2) {
            return -0.2;
        }
        if (modifier < 2 * n / 3) {
            return +0.16;
        }
        if (modifier < 5 * n / 6) {
            return 0.16;
        }
        return -0.2;
    }();

    for (size_t i = 0; i < num; ++i) {
        shoot_circular(BulletType::Seed_Red, radius, start_angle + offset + delta * i);
    }
}

void TestBoss::shoot() {
    if (!phase_) {
        return;
    }

    // if (!phase_->started) {
        hitbox_->activate();
    //     phase_->started = true;
    // }

    // if (phase_->started) {
        shot_num_++;
        play_sound("boss_shoot");
        phase_->shoot();
    // }
}

void TestBoss::update(float deltaTime) {
    AnimatedBoss::update(deltaTime);

    static bool flag = false;
    
    if (hp_ <= 0) {
        switch (phase_num_) {
        case 1:
            changePhase(new BallBounce(this));
            break;
        case 2:
            changePhase(new StreamsRandom(this));
            break;
        case 3:
            changePhase(new Chaos(this));
            break;
        case 4:
            changePhase(new ChessHoming(this));
            break;
        case 5:
            changePhase(new CircularGrouped(this));
            break;
        case 6:
            changePhase(new CircularRotating(this));
            break;
        default:
            if (flag)
                break;
            setAnimation(sprites_["death"]);
            play_sound("boss_death");
            changePhase(nullptr);
            flag = true;
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
