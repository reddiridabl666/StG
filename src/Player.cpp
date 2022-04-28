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

 void Player::on_collide(const DynamicObject* obj) {
    DynamicObject::on_collide(obj);
    if (obj->getTag() == Tag::Wall) {
        if (obj == &Wall::Bounds.left) {
        setPosition(Wall::Bounds.left.getPosition().x + 
                    Wall::Bounds.left.getSize().x / 2 + hitbox_->getSize().x / 2,
                    getPosition().y);
        }
        if (obj == &Wall::Bounds.right) {
            setPosition(Wall::Bounds.right.getPosition().x - 
                        Wall::Bounds.right.getSize().x / 2 - hitbox_->getSize().x / 2,
                        getPosition().y);
        }
        if (obj == &Wall::Bounds.up) {
            setPosition(getPosition().x, Wall::Bounds.up.getPosition().y - 
                        Wall::Bounds.up.getSize().y / 2 + hitbox_->getSize().y / 2);
        }
        if (obj == &Wall::Bounds.low) {
            setPosition(getPosition().x, Wall::Bounds.low.getPosition().y + 
                        Wall::Bounds.low.getSize().y / 2 - hitbox_->getSize().y / 2);
        }
    }
 }

void Player::control() {
    setVelocity({0, 0});
    // Change later!!!
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
