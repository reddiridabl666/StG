#include "Player.h"
#include "Wall.hpp"

using Key = sf::Keyboard;

static bool pressed_any_of(Key::Key A, Key::Key B) {
    return Key::isKeyPressed(A) ||
           Key::isKeyPressed(B);
}

Player::Player(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f hitbox_size, 
               float speed, float mass, Layer layer) 
    : DynamicObject(texture, pos, hitbox_size, {0, 0}, mass, layer), speed_(speed) {
    // setTag(Tag::Player);
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
    setVelocity({0, 0});
    // TODO: Literal constants are bad, change later!!!
    if (pressed_any_of(Key::LShift, Key::RShift)) {
        speed_ = 320.f;
    } else {
        speed_ = 750.f;
    }
    if (pressed_any_of(Key::A, Key::Left)) {
        // this->move(left * speed_ * deltaTime);
        setVelocity(velocity_ + left * speed_);
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        // this->move(right * speed_ * deltaTime);
        setVelocity(velocity_ + right * speed_);
    }
    if (pressed_any_of(Key::W, Key::Up)) {
        // this->move(up * speed_ * deltaTime);
        setVelocity(velocity_ + up * speed_);
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        // this->move(down * speed_ * deltaTime);
        setVelocity(velocity_ + down * speed_);
    }
}
