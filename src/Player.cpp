#include "Player.h"

static bool pressed_any_of(sf::Keyboard::Key A, sf::Keyboard::Key B) {
    return sf::Keyboard::isKeyPressed(A) ||
           sf::Keyboard::isKeyPressed(B);
}

Player::Player(const sf::Texture& texture, sf::Vector2f hitbox_size, 
        Layer layer, float speed) : HitboxObject(texture, hitbox_size, layer), speed_(speed) {}

void Player::control(float deltaTime) {
    using Key = sf::Keyboard;

    // Change later!!!
    if (pressed_any_of(Key::LShift, Key::RShift)) {
        speed_ = 320.f;
    } else {
        speed_ = 750.f;
    }
    if (pressed_any_of(Key::A, Key::Left)) {
        this->move(left * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        this->move(right * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::W, Key::Up)) {
        this->move(up * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        this->move(down * speed_ * deltaTime);
    }
}
