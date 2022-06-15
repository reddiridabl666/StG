#include "GameState.hpp"

#include "Wall.hpp"
#include "Window.h"
#include "Text.hpp"
#include "Player.hpp"
#include "Resources.hpp"

GameState GameState::state;

void GameState::Settings_::volumeUp(sf::Uint8 vol) {
    volume = volume + vol > 100 ? 100 : volume + vol;
}

void GameState::Settings_::volumeDown(sf::Uint8 vol) {
    volume = volume - vol < 0 ? 0 : volume - vol;
}

void GameState::Settings_::setShoot(Key::Key k, Gamepad::Button g) {
    k_shoot = k;
    g_shoot = g;
}

void GameState::Settings_::setSlow(Key::Key k, Gamepad::Button g) {
    k_slow = k;
    g_slow = g;
}

void GameState::init(const Window* window, const Frame& frame, float offset) {
    state.window_ = window;

    state.bounds_ = {frame.left.getPosition().x - offset, 
                     frame.up.getPosition().y - offset,
                     frame.right.getPosition().x + offset - state.bounds_.left,
                     frame.low.getPosition().y + offset - state.bounds_.top};           
}

void GameState::setPlayerTracking(std::weak_ptr<Player>& player) {
    state.player_ = player;
}

sf::Vector2f GameState::getPlayerPos() {
    return player() ? player()->getPosition() : sf::Vector2f();
}

