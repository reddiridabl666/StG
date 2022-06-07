#pragma once

#include "ShootingObject.hpp"
#include "Animated.hpp"

class Enemy;

class PlayerBullet : public Bullet {
public:
    PlayerBullet(Layer layer = Layer::Bullet) : Bullet(layer) {setTag(Tag::PlayerBullet);}

    PlayerBullet(Bullet::Info info, Layer layer = Layer::Bullet) : Bullet(info, layer) {
        setTag(Tag::PlayerBullet);
    }
};

class Player : public ShootingObject<PlayerBullet>, public Animated {
protected:
    float speed_ = 800;
    float normal_speed_ = speed_;
    float slow_speed_ = 350;
    
    BulletType normal_shot_ = BulletType::Player;

    sf::Clock invinc_clock_;
    sf::Clock flick_clock_;
    float flick_time = 0.1;
    float invinc_time_ = 1.5;

    void init_sprites(sf::Image sprite_sheet) override;
    void control();

public:
    Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player& other) = delete;

    Player(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
           sf::Vector2f hitbox_size = {0, 0}, float speed = 800, 
           float mass = 0, Layer layer = Layer::Character);

    // void on_collide(DynamicObject* obj) override;
    // void on_collide_stop() override;

    void setTexture(const sf::Texture& texture) override {
        ShootingObject::setTexture(texture);
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
        if (speed_ == slow_speed_ && hitbox_) {
            hitbox_->show();
        }
    }

    bool is_invincible() const {
        return !hitbox_->is_active();
    }

    float get_invinc_time() const {
        return is_invincible() ? invinc_time_ - invinc_clock_.getElapsedTime().asSeconds() : 0;
    }
};
