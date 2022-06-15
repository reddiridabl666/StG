#include "Player.hpp"
#include "Window.h"
#include "Enemy.hpp"
#include "UpdateFunctions.hpp"
#include "Resources.hpp"
#include "Controls.hpp"

static const sf::Vector2f player_hitbox_size = {30, 45};
static constexpr float player_size = 32 * 4.4;

static Animated::Sprites init_sprites(const sf::Image& sprite_sheet) {
    std::unordered_map<std::string, std::vector<sf::Texture>> sprites_;
    sprites_["idle"] = load_row(sprite_sheet, 4, {0, 0});
    sprites_["right"] = load_row(sprite_sheet, 3, {0, 32});
    sprites_["left"] = load_row(sprite_sheet, 3, {0, 64});
    sprites_["death"] = load_row(sprite_sheet, 9, {0, 96});
    return sprites_;
}


static float vertical_movement(unsigned int gamepad_num = 0) {
    float vertical = 0;
    if (pressed_any_of(Key::W, Key::Up)) {
        vertical = -1;
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        vertical = 1;
    }
    return vertical ? vertical : gamepad_movement(Axis::Y, Axis::PovY, 0, gamepad_num);
}

static float horizontal_movement(unsigned int gamepad_num = 0) {
    float horizontal = 0;
    if (pressed_any_of(Key::A, Key::Left)) {
        horizontal = -1;
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        horizontal = 1;
    }
    return horizontal ? horizontal : gamepad_movement(Axis::X, Axis::PovX, 0, gamepad_num);
} 

Player::Player(sf::Vector2f pos,
               sf::Vector2f hitbox_size, float speed, 
               float mass, Layer layer) :
    Animated(init_sprites(Resources::sprite_sheets["player"])),
    ShootingObject(sprites_["idle"][0], pos, hitbox_size, {0, 0}, mass, layer), speed_(speed),
    health_bar_("Health: ", hp_, 
               {GameState::window()->getView().getSize().x - 270, 50}, 
                48, Layer::Hitbox) {

    auto factor = player_size / min(sprites_["idle"][0].getSize());
    scale(factor, factor);

    setTag(Tag::Player);
    setHP(10);
    
#ifdef DEBUG
#include <limits>
    setHP(std::numeric_limits<int>::max());
#endif
}

void Player::on_collide(Bullet* bullet) {
    if (!bullet || is_invincible()) {
        return;
    }
    if (bullet->getTag() == Tag::Enemy) {
        ShootingObject::on_collide(bullet);
    }
}

void Player::on_collide(Enemy* enemy) {
    if (!enemy || is_invincible()) {
        return;
    }
    enemy->dealDamage(*this);
}

void Player::control() {
    if (hp_ <= 0) {
        setVelocity(0, 0);
        return;
    }

    if (pressed_any_of(Key::LShift, Key::RShift) || Joy::isButtonPressed(0, Gamepad::RB)) {
        speed_ = slow_speed_;
        if (!is_invincible()) hitbox_->show();
    } else {
        speed_ = normal_speed_;
#ifndef DEBUG
        hitbox_->hide();
#endif
    }

    setVelocity(speed_ * (horizontal_movement() * right + vertical_movement() * down));

    if (shoot_clock_ >= shot_interval && 
            (Key::isKeyPressed(Key::Space) || Joy::isButtonPressed(0, Gamepad::A))) {
        Player::shoot(normal_shot_);
    }
}

void Player::update() {
    if (!hitbox_->is_active()) {
        if (flick_clock_.getElapsedTime().asSeconds() >= flick_time) {
            flick_clock_.restart();
            switch_show_mode();
        }
        if (invinc_clock_.getElapsedTime().asSeconds() >= invinc_time_) {
            hitbox_->activate();
            show();
        }
    }

    control();
    
    if (hp_ > 0) {
        if (getVelocity().x > 0) {
            setAnimation(sprites_["right"]);
        }
        if (getVelocity().x < 0) {
            setAnimation(sprites_["left"]);
        }
        if (getVelocity().x == 0) {
            setAnimation(sprites_["idle"]);
        }
    } else if (!flag) {
        setAnimation(sprites_["death"]);
        play_sound("player_death");
        hitbox_->hide();
        flag = true;
    }

    Animated::update();

    if (getTexture() == &sprites_["death"][8]) {
        deactivate();
    }
}

void Player::shoot(BulletType name) {
    shoot_clock_ = 0;
    play_sound("player_shoot", 0.5);

    auto& bullet = 
    gen_.shoot(Bullet::Types[name], getPosition() - sf::Vector2f{15, 70});
    bullet->setDamage(10000);
    auto& bullet2 = 
    gen_.shoot(Bullet::Types[name], getPosition() - sf::Vector2f{-15, 70});
    bullet2->setDamage(10000);
}
