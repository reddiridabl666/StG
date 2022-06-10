#include "Player.hpp"
#include "Wall.hpp"
#include "Enemy.hpp"
#include "UpdateFunctions.hpp"
#include "Resources.hpp"

using Key = sf::Keyboard;
using Gamepad = sf::Joystick;
using Axis = Gamepad::Axis;

static const sf::Vector2f player_hitbox_size = {30, 45};
static constexpr float player_size = 32 * 4.4;

void Player::init_sprites(sf::Image sprite_sheet) {
    sprites_["idle"] = load_row(sprite_sheet, 4, {0, 0});
    sprites_["right"] = load_row(sprite_sheet, 3, {0, 32});
    sprites_["left"] = load_row(sprite_sheet, 3, {0, 64});
}

static float gamepad_movement(Axis axis, unsigned int gamepad_num = 0) {
    float pos = Gamepad::getAxisPosition(gamepad_num, axis);
    if (std::abs(pos) > 0) {
        return pos > 0 ? 1 : -1;
    }
    return 0;
}

static float gamepad_movement(Axis axis1, Axis axis2, unsigned int gamepad_num = 0) {
    return sign(gamepad_movement(axis1, gamepad_num) + gamepad_movement(axis2, gamepad_num));
}

static bool pressed_any_of(Key::Key A, Key::Key B) {
    return Key::isKeyPressed(A) ||
           Key::isKeyPressed(B);
}

static float vertical_movement(unsigned int gamepad_num = 0) {
    float vertical = 0;
    if (pressed_any_of(Key::W, Key::Up)) {
        vertical = -1;
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        vertical = 1;
    }
    return vertical ? vertical : gamepad_movement(Axis::Y, Axis::PovY, gamepad_num);
}

static float horizontal_movement(unsigned int gamepad_num = 0) {
    float horizontal = 0;
    if (pressed_any_of(Key::A, Key::Left)) {
        horizontal = -1;
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        horizontal = 1;
    }
    return horizontal ? horizontal : gamepad_movement(Axis::X, Axis::PovX, gamepad_num);
} 

Player::Player(const sf::Texture& texture, sf::Vector2f pos,
               sf::Vector2f hitbox_size, float speed, 
               float mass, Layer layer) : 
    ShootingObject(texture, pos, hitbox_size, {0, 0}, mass, layer), speed_(speed) {
    auto factor = player_size / min(texture.getSize());
    scale(factor, factor);
    setTag(Tag::Player);
    init_sprites(Resources::sprite_sheets["player"]);
#ifdef DEBUG
#include <limits>
    setHP(std::numeric_limits<int>::max());
#endif
}

// void Player::on_collide_stop() {
//     DynamicObject::on_collide_stop();
// }

// void Player::on_collide(DynamicObject* obj) {
//     DynamicObject::on_collide(obj);
    
//     if (obj->getTag() == Tag::Enemy && hitbox_->is_active()) {
//         // loseHP();
//         if (auto damage_source = dynamic_cast<DamageDealing*>(obj)) {
//             damage_source->dealDamage(*this);
//             invinc_clock_.restart();
//             flick_clock_.restart();
//             hitbox_->deactivate();
//         }
//     }
// }

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
    if (pressed_any_of(Key::LShift, Key::RShift) || Gamepad::isButtonPressed(0, 5)) {
        speed_ = slow_speed_;
        if (!is_invincible()) hitbox_->show();
    } else {
        speed_ = normal_speed_;
#ifndef DEBUG
        hitbox_->hide();
#endif
    }

    setVelocity(speed_ * (horizontal_movement() * right + vertical_movement() * down));

    if (shoot_clock_.getElapsedTime().asSeconds() >= shot_interval && 
            (Key::isKeyPressed(Key::Space) || Gamepad::isButtonPressed(0, 0))) {
        Player::shoot(normal_shot_);
    }
}

void Player::update() {
    // setVelocity(0, 0);

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

    if (getVelocity().x > 0) {
        setAnimation(sprites_["right"]);
    }
    if (getVelocity().x < 0) {
        setAnimation(sprites_["left"]);
    }
    if (getVelocity().x == 0) {
        setAnimation(sprites_["idle"]);
    }
    Animated::update();
}

void Player::shoot(BulletType name) {
    shoot_clock_.restart();

    gen_.shoot(Bullet::Types[name], getPosition() - sf::Vector2f{15, 70});
    gen_.shoot(Bullet::Types[name], getPosition() - sf::Vector2f{-15, 70});
}
