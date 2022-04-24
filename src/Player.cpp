#include "Player.h"

static bool pressed_any_of(sf::Keyboard::Key A, sf::Keyboard::Key B) {
    return sf::Keyboard::isKeyPressed(A) ||
           sf::Keyboard::isKeyPressed(B);
}

// template <typename HitboxType>
Player/*<HitboxType>*/::Player(const sf::Texture& texture, sf::Vector2f hitbox_size, 
        Layer layer, float speed) : HitboxObject<RectHitbox/*HitboxType*/>(texture, hitbox_size, layer), speed_(speed) {}

// template <typename HitboxType>
void Player/*<HitboxType>*/::control(float deltaTime) {
    using Key = sf::Keyboard;

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
