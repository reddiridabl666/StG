#pragma once

#include "ShootingObject.hpp"
#include "Animated.hpp"
#include "Audible.hpp"
#include "Text.hpp"

class Enemy;

class PlayerBullet : public Bullet {
public:
    PlayerBullet(Layer layer = Layer::Bullet) : Bullet(layer) {setTag(Tag::PlayerBullet);}

    PlayerBullet(Bullet::Info info, Layer layer = Layer::Bullet) : Bullet(info, layer) {
        setTag(Tag::PlayerBullet);
    }
};

class Player : private Animated, public ShootingObject<PlayerBullet>, private Audible {
protected:
    float speed_ = 800;
    float normal_speed_ = speed_;
    float slow_speed_ = 350;
    
    BulletType normal_shot_ = BulletType::Player;

    sf::Clock invinc_clock_;
    sf::Clock flick_clock_;
    float flick_time = 0.1;
    float invinc_time_ = 1.5;

    Log<sf::Int32> health_bar_;
    bool flag = false;

    static std::pair<Key::Key, Gamepad::Button> shoot_;
    static std::pair<Key::Key, Gamepad::Button> slow_;

    void control();

public:
    Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player& other) = delete;

    Player(sf::Vector2f pos = {0, 0},
           sf::Vector2f hitbox_size = {0, 0}, float speed = 800, 
           float mass = 0, Layer layer = Layer::Character);

    static void setShoot(Key::Key k, Gamepad::Button g) {
        shoot_ = {k, g};
    }

    static void setSlow(Key::Key k, Gamepad::Button g) {
        slow_ = {k, g};
    }

    void setTexture(const sf::Texture& texture) override {
        ShootingObject::setTexture(texture);
    }

    auto& getHealthBar() {
        return health_bar_;
    }

    void on_collide(Bullet* bullet) override;
    void on_collide(Enemy* enemy);
    void on_damage_dealt() {
        invinc_clock_.restart();
        flick_clock_.restart();
        hitbox_->deactivate();
    }

    int loseHP(int hp = 1) override {
        on_damage_dealt();
        return ShootingObject::loseHP(hp);
    }

    void setSpeed(float normal, float slow) {
        normal_speed_ = normal;
        slow_speed_ = slow;
    }

    void update();
    
    void shoot(BulletType name) override;

    void show() override {
        ShootingObject::show();
        if (speed_ == slow_speed_ && hitbox_ && hp_ > 0) {
            hitbox_->show();
        }
    }

    bool is_invincible() const {
        return !hitbox_->is_active();
    }

    float get_invinc_time() const {
        return invinc_time_ - invinc_clock_.getElapsedTime().asSeconds();
    }
};
