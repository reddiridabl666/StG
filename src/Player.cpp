#include "Player.hpp"
#include "Wall.hpp"

using Key = sf::Keyboard;
using Gamepad = sf::Joystick;
using Axis = Gamepad::Axis;

static const sf::Vector2f player_hitbox_size = {30, 45};
static constexpr float player_size = 32 * 4.4;

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
               sf::Vector2f hitbox_size, const std::string bullet_name/*const BulletInfo& info*/,
               float speed, float mass, Layer layer) 
    : ShootingObject(texture, pos, hitbox_size, {0, 0}, mass, layer), speed_(speed), normal_shot_(bullet_name) {
        // gen_.add_bullet("normal", info);
        auto factor = player_size / min(texture.getSize());
        scale(factor, factor);
}

void Player::on_collide_stop() {
    DynamicObject::on_collide_stop();
}

void Player::on_collide(DynamicObject* obj) {
    DynamicObject::on_collide(obj);
    
    if (obj->getTag() == Tag::Enemy && hitbox_->is_active()) {
        // loseHP();
        if (auto damage_source = dynamic_cast<DamageDealing*>(obj)) {
            damage_source->dealDamage(*this);
            invinc_clock_.restart();
            flick_clock_.restart();
            hitbox_->deactivate();
        }
    }

    if (auto wall = dynamic_cast<Wall*>(obj)) {
        // Right wall
        if (wall->is_in_right_sector(this)) {
            setPosition(obj->getPosition().x + obj->getHalfSize().x + 
                        hitbox_->getHalfSize().x, getPosition().y);
        // Left wall
        } else if (wall->is_in_left_sector(this)) {
            setPosition(obj->getPosition().x - obj->getHalfSize().x - 
                        hitbox_->getHalfSize().x, getPosition().y);
        // Upper wall
        } else if (wall->is_in_upper_sector(this)) {
            setPosition(getPosition().x, obj->getPosition().y -
                        obj->getHalfSize().y - hitbox_->getHalfSize().y);
        // Lower wall
        } else if (wall->is_in_lower_sector(this)) {
            setPosition(getPosition().x, obj->getPosition().y +
                        obj->getHalfSize().y + hitbox_->getHalfSize().y);
        }
    }
}

void Player::control() {
    if (pressed_any_of(Key::LShift, Key::RShift) || Gamepad::isButtonPressed(0, 5)) {
        speed_ = slow_speed_;
        // hitbox_->show();
    } else {
        speed_ = normal_speed_;
// #ifndef DEBUG
//         hitbox_->hide();
// #endif
    }

    // float horizontal = 0;
    // float vertical = 0;

    // if (pressed_any_of(Key::A, Key::Left)) {
    //     horizontal = -1;
    //     // setVelocity(velocity_ + left * speed_);
    // }
    // if (pressed_any_of(Key::D, Key::Right)) {
    //     horizontal = 1;
    //     // setVelocity(velocity_ + right * speed_);
    // }
    // if (pressed_any_of(Key::W, Key::Up)) {
    //     vertical = -1;
    //     // setVelocity(velocity_ + up * speed_);
    // }
    // if (pressed_any_of(Key::S, Key::Down)) {
    //     vertical = 1;
    //     // setVelocity(velocity_ + down * speed_);
    // }

    // horizontal = gamepad_movement(Axis::X);
    // vertical = gamepad_movement(Axis::Y);

    setVelocity(speed_ * (horizontal_movement() * right + vertical_movement() * down));

    if (shoot_clock_.getElapsedTime().asSeconds() >= shot_interval && 
            (Key::isKeyPressed(Key::Space) || Gamepad::isButtonPressed(0, 0))) {
        Player::shoot(normal_shot_);
    }
}

void Player::update() {
    setVelocity(0, 0);

    float flick_time = 0.1;
    // auto delta_time = invinc_clock_.getElapsedTime().asSeconds();

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
}

void Player::shoot(std::string name) {
    shoot_clock_.restart();

    sf::Vector2f new_size = {16, 28};

    gen_.shoot(Bullet::BulletTypes[name], getPosition() - sf::Vector2f{15, 40}, 
                        Bullet::BulletTypes[name].velocity, new_size);
    gen_.shoot(Bullet::BulletTypes[name], getPosition() - sf::Vector2f{-15, 40}, 
                        Bullet::BulletTypes[name].velocity, new_size);
}
