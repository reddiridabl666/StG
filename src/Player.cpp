#include "Player.h"
#include "Wall.hpp"

using Key = sf::Keyboard;
static const sf::Vector2f player_hitbox_size = {30, 45};
static constexpr float player_size = 32 * 4.4;

static bool pressed_any_of(Key::Key A, Key::Key B) {
    return Key::isKeyPressed(A) ||
           Key::isKeyPressed(B);
}

Player::Player(const sf::Texture& texture, sf::Vector2f pos,
               sf::Vector2f hitbox_size, const BulletInfo& info,
               float speed, float mass, Layer layer) 
    : ShootingObject(texture, pos, hitbox_size, {0, 0}, mass, layer), speed_(speed) {
        gen_.add_bullet("normal", info);
        auto factor = player_size / min(texture.getSize());
        scale(factor, factor);
}

void Player::on_collide(DynamicObject* obj) {
    DynamicObject::on_collide(obj);
    
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

void Player::setSpeed(float normal, float slow) {
    normal_speed_ = normal;
    slow_speed_ = slow;
}

void Player::control() {
    setVelocity(0, 0);

    if (pressed_any_of(Key::LShift, Key::RShift)) {
        speed_ = slow_speed_;
    } else {
        speed_ = normal_speed_;
    }
    if (pressed_any_of(Key::A, Key::Left)) {
        setVelocity(velocity_ + left * speed_);
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        setVelocity(velocity_ + right * speed_);
    }
    if (pressed_any_of(Key::W, Key::Up)) {
        setVelocity(velocity_ + up * speed_);
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        setVelocity(velocity_ + down * speed_);
    }

    if (clock_.getElapsedTime().asSeconds() > shot_interval && 
            Key::isKeyPressed(Key::Space)) {
        Player::shoot("normal");
    }
}


void Player::shoot(std::string name) {
    clock_.restart();
    gen_.shoot(name, getPosition() - sf::Vector2f{15, 40});
    gen_.shoot(name, getPosition() - sf::Vector2f{-15, 40});
}