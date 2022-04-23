#include "Player.h"

static bool pressed_any_of(sf::Keyboard::Key A, sf::Keyboard::Key B) {
    return sf::Keyboard::isKeyPressed(A) ||
           sf::Keyboard::isKeyPressed(B);
}

Player::Player(const sf::Texture& texture, sf::Vector2f hitbox_size, 
        Layer layer, float speed) : HitboxObject(texture, hitbox_size, layer), speed_(speed) {}

void Player::control(float deltaTime) {
    using Key = sf::Keyboard;

    if (pressed_any_of(Key::A, Key::Left)) {
        move(left * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::D, Key::Right)) {
        move(right * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::W, Key::Up)) {
        move(up * speed_ * deltaTime);
    }
    if (pressed_any_of(Key::S, Key::Down)) {
        move(down * speed_ * deltaTime);
    }
}