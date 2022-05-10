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
    
    if (obj->getTag() == Tag::Wall) {
        float tan = obj->getSize().y  / obj->getSize().x;
        auto diag = [obj] (float x, float tan) {return obj->getPosition().y + (x - obj->getPosition().x) * tan;};
        auto diag2 = [obj] (float x, float tan) {return obj->getPosition().x + (x - obj->getPosition().y) * tan;};

        float tan2 = obj->getSize().x  / obj->getSize().y;
        // Right wall
        if (getPosition().x >= obj->getPosition().x + obj->getSize().x / 2 &&
            obj->getPosition().y <= diag(obj->getPosition().x, tan) && 
            obj->getPosition().y >= diag(obj->getPosition().x, tan2)) {
            setPosition(obj->getPosition().x + obj->getSize().x / 2 + 
                        hitbox_->getSize().x / 2, getPosition().y);
        // Left wall
        } else if (getPosition().x <= obj->getPosition().x - obj->getSize().x / 2 &&
                   obj->getPosition().y >= diag(obj->getPosition().x, tan) && 
                   obj->getPosition().y <= diag(obj->getPosition().x, tan2)) {
            setPosition(obj->getPosition().x - obj->getSize().x / 2 - 
                        hitbox_->getSize().x / 2, getPosition().y);
        // Upper wall
        } else if (getPosition().y >= obj->getPosition().y + obj->getSize().y / 2 &&
                   obj->getPosition().x <= diag2(obj->getPosition().y, 1 / tan) && 
                   obj->getPosition().x >= diag2(obj->getPosition().y, 1 / tan2)) {
            setPosition(getPosition().x, obj->getPosition().y + 
                        obj->getSize().y / 2 + hitbox_->getSize().y / 2);
        // Lower wall
        } else if (getPosition().y <= obj->getPosition().y - obj->getSize().y / 2 &&
                   obj->getPosition().x <= diag2(obj->getPosition().y, 1 / tan) && 
                   obj->getPosition().x >= diag2(obj->getPosition().y, 1 / tan2)) {
            setPosition(getPosition().x, obj->getPosition().y -
                        obj->getSize().y / 2 - hitbox_->getSize().y / 2);
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
